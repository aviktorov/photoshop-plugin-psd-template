#include <sstream>

#include "PIActionsPlugIn.h"
#include "PIUFile.h"
#include "PIUGet.h"
#include "PIUI.h"

#include "PITerminology.h"

#include "TemplateAutomation.h"
#include "TemplateAutomationUI.h"

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
	PIActionDescriptor action = NULL;
	SPErr error = sPSActionDescriptor->Make(&action);
	if(error) goto returnError;
	
	PIActionList layer_list = NULL;
	error = sPSActionList->Make(&layer_list);
	if(error) goto returnError;
	
	PIActionReference layer = NULL;
	error = sPSActionReference->Make(&layer);
	if(error) goto returnError;
	
	ASZString temp_str;
	sASZString->MakeFromUnicode((ASUnicode*)&name[0],name.size(),&temp_str);
	error = sPSActionReference->PutNameZString(layer,classLayer,temp_str);
	if(error) goto returnError;
	
	error = sPSActionList->PutReference(layer_list,layer);
	if(error) goto returnError;
	
	error = sPSActionDescriptor->PutList(action,keyNull,layer_list);
	if(error) goto returnError;
	
	PIActionDescriptor result = NULL;
	error = sPSActionControl->Play(&result,(hidden) ? eventHide : eventShow,action,plugInDialogSilent);
	if(error) goto returnError;
	
	returnError:
	
	if(result != NULL) sPSActionDescriptor->Free(result);
	if(action != NULL) sPSActionDescriptor->Free(action);
	if(layer_list != NULL) sPSActionList->Free(layer_list);
	if(layer != NULL) sPSActionReference->Free(layer);
	
	return error;
}

SPErr SetLayerText(const std::wstring& name,const std::wstring& text) {
	PIActionDescriptor result = NULL;
	
	PIActionDescriptor action = NULL;
	SPErr error = sPSActionDescriptor->Make(&action);
	if(error) goto returnError;
	
	PIActionReference layer = NULL;
	error = sPSActionReference->Make(&layer);
	if(error) goto returnError;
	
	ASZString temp_str;
	sASZString->MakeFromUnicode((ASUnicode*)&name[0],name.size(),&temp_str);
	error = sPSActionReference->PutNameZString(layer,classTextLayer,temp_str);
	if(error) goto returnError;
	
	error = sPSActionDescriptor->PutReference(action,keyNull,layer);
	if(error) goto returnError;
	
	PIActionDescriptor layer_text = NULL;
	error = sPSActionDescriptor->Make(&layer_text);
	if(error) goto returnError;
	
	sASZString->MakeFromUnicode((ASUnicode*)&text[0],text.size(),&temp_str);
	error = sPSActionDescriptor->PutZString(layer_text,keyText,temp_str);
	if(error) goto returnError;
	
	error = sPSActionDescriptor->PutObject(action,keyTo,classTextLayer,layer_text);
	if(error) goto returnError;
	
	error = sPSActionControl->Play(&result,eventSet,action,plugInDialogSilent);
	if(error) goto returnError;
	
	returnError:
	
	if(result != NULL) sPSActionDescriptor->Free(result);
	if(action != NULL) sPSActionDescriptor->Free(action);
	if(layer != NULL) sPSActionReference->Free(layer);
	if(layer_text != NULL) sPSActionDescriptor->Free(layer_text);
	
	return error;
}

/*
 */
SPErr SavePSD(const char* name) {
	PIActionDescriptor action = NULL;
	SPErr error = sPSActionDescriptor->Make(&action);
	if(error) goto returnError;
	
	PIActionDescriptor compatibility = NULL;
	error = sPSActionDescriptor->Make(&compatibility);
	if(error) goto returnError;
	
	DescriptorTypeID compatibility_key;
	error = sPSActionControl->StringIDToTypeID("maximizeCompatibility",&compatibility_key);
	if(error) goto returnError;
	
	error = sPSActionDescriptor->PutBoolean(compatibility,compatibility_key,true);
	if(error) goto returnError;
	
	error = sPSActionDescriptor->PutObject(action,keyAs,classPhotoshop35Format,compatibility);
	if(error) goto returnError;
	
	Handle path = NULL;
	FullPathToAlias(name,path);
	
	error = sPSActionDescriptor->PutAlias(action,keyIn,path);
	if(error) goto returnError;
	
	PIActionDescriptor result = NULL;
	error = sPSActionControl->Play(&result,eventSave,action,plugInDialogSilent);
	if(error) goto returnError;
	
	returnError:
	
	if(compatibility != NULL) sPSActionDescriptor->Free(compatibility);
	if(result != NULL) sPSActionDescriptor->Free(result);
	if(action != NULL) sPSActionDescriptor->Free(action);
	if(path != NULL) sPSHandle->Dispose(path);
	
	return error;
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
