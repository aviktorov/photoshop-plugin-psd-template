#include "TemplateAutomation.h"
#include "TemplateAutomationUI.h"

DLLExport BOOL WINAPI TemplateAutomationProc(HWND hDlg,UINT wMsg,WPARAM wParam,LPARAM lParam);

SPErr DoUI() {
	INT_PTR item = DialogBoxParam(
		GetDLLInstance(gPlugInRef),
		MAKEINTRESOURCE(TemplateAutomationDialogID),
		GetActiveWindow(),
		reinterpret_cast<DLGPROC>(TemplateAutomationProc),
		NULL
	);
	
	if(item != kDOk_button) return 'STOP';
	
	return noErr;
}

DLLExport BOOL WINAPI TemplateAutomationProc(HWND hDlg,UINT wMsg,WPARAM wParam,LPARAM) {
	switch(wMsg) {
		case WM_INITDIALOG: {
			CenterDialog(hDlg);
			DoUIInit(hDlg);
			return TRUE;
		}
		
		case WM_COMMAND: {
			int item = LOWORD(wParam);
			int cmd = HIWORD(wParam);
			
			switch(item)
			{
				case kDOk_button: {
					if(cmd == BN_CLICKED) {
						EndDialog(hDlg, item);
						return TRUE;
					}
				} break;
				
				case kDCancel_button: {
					if(cmd == BN_CLICKED) {
						EndDialog(hDlg, item);
						return TRUE;
					}
				} break;
			}
			
			return TRUE;
		}
		
		default: {
			return FALSE;
		} break;
	}
	
	return TRUE;
}
