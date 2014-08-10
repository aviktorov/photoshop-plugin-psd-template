#include <sstream>

#include "TemplateAutomation.h"
#include "TemplateAutomationUI.h"
#include "CSVData.h"

#include <ShlObj.h>

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
			
			if(cmd == BN_CLICKED) {
				switch(item) {
					case kDOk_button: {
						std::string csv_path;
						csv_text.GetText(csv_path);
						
						if(csv_path.empty()) {
							MessageBox(hDlg,"CSV path is empty","Error",MB_OK);
							return TRUE;
						}
						
						std::string save_dir;
						save_dir_text.GetText(save_dir);
						
						if(save_dir.empty()) {
							MessageBox(hDlg,"Save dir is empty","Error",MB_OK);
							return TRUE;
						}
						
						std::string save_name;
						save_name_text.GetText(save_name);
						
						if(save_name.empty()) {
							MessageBox(hDlg,"Save name is empty","Error",MB_OK);
							return TRUE;
						}
						
						CSVData csv_data;
						if(csv_data.load(csv_path) == 0) {
							MessageBox(hDlg,"Can't load CSV file","Error",MB_OK);
							return TRUE;
						}
						
						if(csv_data.getNumRows() == 0) {
							MessageBox(hDlg,"CSV file is empty","Error",MB_OK);
							return TRUE;
						}
						
						const csv_row& header = csv_data.getRow(0);
						
						for(size_t i = 1; i < csv_data.getNumRows(); ++i) {
							std::ostringstream psd_stream;
							psd_stream << save_dir.c_str() << '\\' << save_name.c_str() << "_" << i << ".psd";
							
							std::string psd_name = psd_stream.str();
							const csv_row& data = csv_data.getRow(i);
							
							MakePSDTemplate(psd_name.c_str(),header,data);
						}
					}
					
					case kDCancel_button: {
						EndDialog(hDlg, item);
						return TRUE;
					} break;
					
					case kDCSV_choose_button: {
						char file[MAX_PATH] = "";
						
						OPENFILENAME dialog_settings;
						ZeroMemory(&dialog_settings, sizeof(dialog_settings));
						
						dialog_settings.lStructSize = sizeof(dialog_settings);
						dialog_settings.hwndOwner = hDlg;
						dialog_settings.lpstrFilter = "All files (*.*)\0*.*\0csv files (*.csv)\0*.csv\0";
						dialog_settings.lpstrCustomFilter = NULL;
						dialog_settings.nFilterIndex = 2;
						dialog_settings.lpstrTitle = "Select template data";
						dialog_settings.Flags = OFN_FILEMUSTEXIST;
						dialog_settings.lpstrFile = file;
						dialog_settings.lpstrFile[0] = '\x0';
						dialog_settings.nMaxFile = sizeof(file);
						
						if(GetOpenFileName(&dialog_settings) == TRUE) {
							csv_text.SetText(dialog_settings.lpstrFile);
						}
						
						return TRUE;
					} break;
					
					case kDSaveDir_choose_button: {
						BROWSEINFO dialog_settings;
						ZeroMemory(&dialog_settings, sizeof(dialog_settings));
						
						dialog_settings.hwndOwner = hDlg;
						dialog_settings.lpszTitle = "Pick a Directory";
						dialog_settings.ulFlags = BIF_NEWDIALOGSTYLE;
						
						LPITEMIDLIST pidl = SHBrowseForFolder(&dialog_settings);
						if(pidl != 0) {
							char path[MAX_PATH];
							if(SHGetPathFromIDList(pidl,path)) {
								save_dir_text.SetText(path);
							}
							
							IMalloc* imalloc = 0;
							if(SUCCEEDED(SHGetMalloc(&imalloc))) {
								imalloc->Free(pidl);
								imalloc->Release();
							}
						}
						
						return TRUE;
					} break;
				}
			}
			
			return TRUE;
		}
		
		default: {
			return FALSE;
		} break;
	}
	
	return TRUE;
}
