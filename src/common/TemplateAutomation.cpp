#include <sstream>

#include "PIActionsPlugIn.h"
#include "PIUFile.h"
#include "PIUGet.h"
#include "PIUI.h"

#include "PITerminology.h"

#include "TemplateAutomation.h"
#include "TemplateAutomationUI.h"

#include "WikiParser.h"

/*
 */
SPBasicSuite *sSPBasic = NULL;
SPPluginRef	gPlugInRef = NULL;

/*
 */
static void Initialize();

static SPErr Startup();
static SPErr Shutdown();
static SPErr Execute(PIActionParameters *actionParams);

/*
 */
SPErr PutNameWString(PIActionReference ref,DescriptorClassID class_id,const std::wstring& str) {
	ASZString temp_str;
	sASZString->MakeFromUnicode((ASUnicode*)&str[0],str.size(),&temp_str);
	return sPSActionReference->PutNameZString(ref,class_id,temp_str);
}

/*
 */
SPErr PutWString(PIActionDescriptor desc,DescriptorKeyID key_id,const std::wstring& str) {
	ASZString temp_str;
	sASZString->MakeFromUnicode((ASUnicode*)&str[0],str.size(),&temp_str);
	return sPSActionDescriptor->PutZString(desc,key_id,temp_str);
}

/*
 */
SPErr SetLayerHidden(const std::wstring& name,bool hidden) {
	Auto_Ref layer_ref;
	SPErr error = PutNameWString(layer_ref.get(),classLayer,name);
	if(error) return error;
	
	Auto_List layer_list;
	error = sPSActionList->PutReference(layer_list.get(),layer_ref.get());
	if(error) return error;
	
	Auto_Desc action;
	error = sPSActionDescriptor->PutList(action.get(),keyNull,layer_list.get());
	if(error) return error;
	
	Auto_Desc result(false);
	return sPSActionControl->Play(&result,(hidden) ? eventHide : eventShow,action.get(),plugInDialogSilent);
}

SPErr SetLayerText(const std::wstring& name,const std::wstring& text) {
	WikiParser parser;
	parser.parse(text);
	
	Auto_Ref layer_ref;
	SPErr error = PutNameWString(layer_ref.get(),classTextLayer,name);
	if(error) return error;
	
	Auto_Desc layer_text;
	error = PutWString(layer_text.get(),keyText,parser.getProcessedString());
	if(error) return error;
	
	Auto_List text_style_list;
	for(size_t i = 0; i < parser.getNumBoldTokens(); ++i) {
		Auto_Desc text_range;
		error = sPSActionDescriptor->PutInteger(text_range.get(),keyFrom,parser.getBoldStart(i));
		if(error) return error;
		
		error = sPSActionDescriptor->PutInteger(text_range.get(),keyTo,parser.getBoldStart(i));
		if(error) return error;
		
		Auto_Desc text_style;
		
		DescriptorKeyID keyBold = 0;
		error = sPSActionControl->StringIDToTypeID("syntheticBold",&keyBold);
		if(error) return error;
		
		error = sPSActionDescriptor->PutBoolean(text_style.get(),keyBold,true);
		if(error) return error;
		
		error = sPSActionDescriptor->PutObject(text_range.get(),keyTextStyle,classTextStyle,text_style.get());
		if(error) return error;
		
		error = sPSActionList->PutObject(text_style_list.get(),classTextStyleRange,text_range.get());
		if(error) return error;
	}
	
	for(size_t i = 0; i < parser.getNumItalicTokens(); ++i) {
		Auto_Desc text_range;
		error = sPSActionDescriptor->PutInteger(text_range.get(),keyFrom,parser.getItalicStart(i));
		if(error) return error;
		
		error = sPSActionDescriptor->PutInteger(text_range.get(),keyTo,parser.getItalicStart(i));
		if(error) return error;
		
		Auto_Desc text_style;
		
		DescriptorKeyID keyItalic = 0;
		error = sPSActionControl->StringIDToTypeID("synthetictalic",&keyItalic);
		if(error) return error;
		
		error = sPSActionDescriptor->PutBoolean(text_style.get(),keyItalic,true);
		if(error) return error;
		
		error = sPSActionDescriptor->PutObject(text_range.get(),keyTextStyle,classTextStyle,text_style.get());
		if(error) return error;
		
		error = sPSActionList->PutObject(text_style_list.get(),classTextStyleRange,text_range.get());
		if(error) return error;
	}
	
	Auto_Desc action;
	error = sPSActionDescriptor->PutReference(action.get(),keyNull,layer_ref.get());
	if(error) return error;
	
	error = sPSActionDescriptor->PutObject(action.get(),keyTo,classTextLayer,layer_text.get());
	if(error) return error;
	
	Auto_Desc result(false);
	return sPSActionControl->Play(&result,eventSet,action.get(),plugInDialogSilent);
}

/*
 */
SPErr SavePSD(const char* name) {
	DescriptorTypeID compatibility_key;
	SPErr error = sPSActionControl->StringIDToTypeID("maximizeCompatibility",&compatibility_key);
	if(error) return error;
	
	Auto_Desc compatibility;
	error = sPSActionDescriptor->PutBoolean(compatibility.get(),compatibility_key,true);
	if(error) return error;
	
	Auto_Desc action;
	error = sPSActionDescriptor->PutObject(action.get(),keyAs,classPhotoshop35Format,compatibility.get());
	if(error) return error;
	
	Handle path = NULL;
	FullPathToAlias(name,path);
	
	error = sPSActionDescriptor->PutAlias(action.get(),keyIn,path);
	if(path) sPSHandle->Dispose(path);
	
	if(error) return error;
	
	Auto_Desc result(false);
	return sPSActionControl->Play(&result,eventSave,action.get(),plugInDialogSilent);
}

/*
 */
int MakePSDTemplate(const char* name,const csv_row& header,const csv_row& data) {
	for(size_t i = 0; i < data.size(); ++i) {
		SetLayerHidden(data[i],false);
	}
	
	for(size_t i = 0; i < header.size(); ++i) {
		SetLayerText(header[i],data[i]);
	}
	
	SPErr result = SavePSD(name);
	
	for(size_t i = 0; i < data.size(); ++i) {
		SetLayerHidden(data[i],true);
	}
	
	return (result == kNoErr);
}

/*
 */
DLLExport SPAPI SPErr AutoPluginMain(const char* caller,const char* selector,void* message) {
	SPErr error = kSPNoError;
	
	try {
		SPMessageData *basicMessage = reinterpret_cast<SPMessageData*>(message);
		sSPBasic = basicMessage->basic;
		gPlugInRef = basicMessage->self;
		
		if(sSPBasic->IsEqual(caller,kSPInterfaceCaller)) {
			if(sSPBasic->IsEqual(selector,kSPInterfaceAboutSelector)) DoAbout(gPlugInRef,TemplateAutomationAboutID);
			if(sSPBasic->IsEqual(selector,kSPInterfaceStartupSelector)) error = Startup();
			if(sSPBasic->IsEqual(selector,kSPInterfaceShutdownSelector)) error = Shutdown();
		}
		
		if(sSPBasic->IsEqual(caller, kPSPhotoshopCaller)) {
			if(sSPBasic->IsEqual(selector,kPSDoIt)) {
				PSActionsPlugInMessage *actionsMessage = reinterpret_cast<PSActionsPlugInMessage*>(message);
				PIActionParameters *actionParams = actionsMessage->actionParameters;
				error = Execute(actionParams);
			}
		}
	}
	catch(...) {
		if(error == kSPNoError) error = kSPBadParameterError;
	}
	
	return error;
}

/*
 */
static void Initialize() {
	// TODO: initialize here
}

static SPErr Startup() {
	// TODO: startup here
	
	return kSPNoError;
}

static SPErr Shutdown() {
	// TODO: shutdown here
	PIUSuitesRelease();
	
	return kSPNoError;
}

static SPErr Execute(PIActionParameters *actionParams) {
	SPErr error = kSPNoError;
	
	Initialize(); 
	
	PIDialogPlayOptions playInfo = actionParams->playInfo;
	if(playInfo == plugInDialogDisplay) error = DoUI();
	
	return error;
}
