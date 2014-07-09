#include <sstream>

#include "TemplateAutomation.h"
#include "TemplateAutomationUI.h"
#include "csv_parser.hpp"

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
							MessageBox(NULL,"CSV path is empty","Error",MB_OK);
							return TRUE;
						}
						
						std::string template_name;
						template_text.GetText(template_name);
						
						if(template_name.empty()) {
							MessageBox(NULL,"Template name is empty","Error",MB_OK);
							return TRUE;
						}
						
						csv_parser file_parser;
						if(!file_parser.init(csv_path.c_str())) {
							MessageBox(NULL,"Can't open CSV file","Error",MB_OK);
							return TRUE;
						}
						
						file_parser.set_enclosed_char('"',ENCLOSURE_OPTIONAL);
						file_parser.set_field_term_char(',');
						file_parser.set_line_term_char('\n');
						
						if(!file_parser.has_more_rows()) {
							MessageBox(NULL,"CSV file is empty","Error",MB_OK);
							return TRUE;
						}
						
						const csv_row& header = file_parser.get_row();
						
						while(file_parser.has_more_rows()) {
							std::ostringstream psd_stream;
							psd_stream << template_name.c_str() << "_" << file_parser.get_record_count() << ".psd";
							psd_stream.flush();
							
							std::string psd_name = psd_stream.str();
							MakePSDTemplate(psd_name.c_str(),header,file_parser.get_row());
						}
					}
					case kDCancel_button: {
						EndDialog(hDlg, item);
						return TRUE;
					} break;
					
					case kDChoose_button: {
						char file[1024] = "";
						
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
