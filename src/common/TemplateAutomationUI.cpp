#include "PITerminology.h"

#include "TemplateAutomation.h"
#include "TemplateAutomationUI.h"

PIText csv_text;
PIText save_dir_text;
PIText save_name_text;

SPErr DoUIInit(PIDialogPtr handle) {
	PIDialog dialog(handle);
	
	csv_text.SetItem(dialog.GetItem(kDCSV_edit));
	save_dir_text.SetItem(dialog.GetItem(kDSaveDir_edit));
	save_name_text.SetItem(dialog.GetItem(kDSaveName_edit));
	
	return kSPNoError;
}
