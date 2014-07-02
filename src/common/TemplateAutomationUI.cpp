#include "PITerminology.h"

#include "TemplateAutomation.h"
#include "TemplateAutomationUI.h"

PIText csv_text;
PIText template_text;

SPErr DoUIInit(PIDialogPtr handle) {
	PIDialog dialog(handle);
	
	csv_text.SetItem(dialog.GetItem(kDCSV_edit));
	template_text.SetItem(dialog.GetItem(kDTemplate_edit));
	
	return kSPNoError;
}
