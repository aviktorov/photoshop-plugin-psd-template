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
void MakePSDTemplate(const char* name,const csv_row& header,const csv_row& data) {
	// TODO: change text layers
	SavePSD(name);
}

/*
 */
static void Initialize();

static SPErr Startup();
static SPErr Shutdown();
static SPErr Execute(PIActionParameters *actionParams);

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
