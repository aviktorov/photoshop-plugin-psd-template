#include <sstream>

#include "PIActionsPlugIn.h"
#include "PIUFile.h"
#include "PIUGet.h"
#include "PIUI.h"

#include "PITerminology.h"

#include "TemplateAutomation.h"
#include "TemplateAutomationUI.h"

#include "Utils.h"
#include "WikiParser.h"
#include "LayerInfo.h"

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
SPErr SetLayerHidden(const std::wstring& name,bool hidden) {
	Auto_Ref layer_ref;
	SPErr error = Utils::PutNameWString(layer_ref.get(),classLayer,name);
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
	
	LayerInfo info;
	SPErr error = info.load(name);
	if(error) return error;
	
	Auto_Ref layer_ref;
	error = Utils::PutNameWString(layer_ref.get(),classTextLayer,name);
	if(error) return error;
	
	Auto_List text_style_list;
	for(size_t i = 0; i < parser.getNumTokens(); ++i) {
		Auto_Desc text_range;
		error = sPSActionDescriptor->PutInteger(text_range.get(),keyFrom,parser.getTokenStart(i));
		if(error) return error;
		
		error = sPSActionDescriptor->PutInteger(text_range.get(),keyTo,parser.getTokenEnd(i));
		if(error) return error;
		
		Auto_Desc text_style;
		
		error = sPSActionDescriptor->PutBoolean(text_style.get(),keyBold,parser.getTokenBold(i));
		if(error) return error;
		
		error = sPSActionDescriptor->PutBoolean(text_style.get(),keyItalic,parser.getTokenItalic(i));
		if(error) return error;
		
		error = info.saveTextStyle(text_style.get());
		if(error) return error;
		
		error = sPSActionDescriptor->PutObject(text_range.get(),keyTextStyle,classTextStyle,text_style.get());
		if(error) return error;
		
		error = sPSActionList->PutObject(text_style_list.get(),classTextStyleRange,text_range.get());
		if(error) return error;
	}
	
	Auto_List paragraph_style_list;
	for(size_t i = 0; i < parser.getNumParagraphs(); ++i) {
		Auto_Desc paragraph_range;
		error = sPSActionDescriptor->PutInteger(paragraph_range.get(),keyFrom,parser.getParagraphStart(i));
		if(error) return error;
		
		error = sPSActionDescriptor->PutInteger(paragraph_range.get(),keyTo,parser.getParagraphEnd(i));
		if(error) return error;
		
		Auto_Desc paragraph_style;
		
		error = info.saveParagraphStyle(paragraph_style.get());
		if(error) return error;
		
		error = sPSActionDescriptor->PutObject(paragraph_range.get(),keyParagraphStyle,keyParagraphStyle,paragraph_style.get());
		if(error) return error;
		
		error = sPSActionList->PutObject(paragraph_style_list.get(),keyParagraphStyleRange,paragraph_range.get());
		if(error) return error;
	}
	
	Auto_Desc layer_text;
	error = Utils::PutWString(layer_text.get(),keyText,parser.getProcessedString());
	if(error) return error;
	
	if(parser.getNumTokens()) {
		error = sPSActionDescriptor->PutList(layer_text.get(),keyTextStyleRange,text_style_list.get());
		if(error) return error;
	}
	
	if(parser.getNumParagraphs()) {
		error = sPSActionDescriptor->PutList(layer_text.get(),keyParagraphStyleRange,paragraph_style_list.get());
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
	Auto_Desc compatibility;
	SPErr error = sPSActionDescriptor->PutBoolean(compatibility.get(),keyMaximizeCompatibility,true);
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
	Utils::Init();
}

static SPErr Startup() {
	return kSPNoError;
}

static SPErr Shutdown() {
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
