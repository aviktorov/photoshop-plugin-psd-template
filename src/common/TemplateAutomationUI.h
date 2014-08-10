#ifndef __TEMPLATE_AUTOMATION_UI_H__
#define __TEMPLATE_AUTOMATION_UI_H__

#include "PIUI.h"

enum {
	kDNoUI = -1,
	kDOk_button = 1,
	kDCancel_button = 2,
	kDCSV_edit = 6,
	kDCSV_choose_button = 7,
	kDSaveDir_edit = 8,
	kDSaveDir_choose_button = 9,
	kDSaveName_edit = 11,
};

SPErr DoUI();
SPErr DoUIInit(PIDialogPtr handle);

extern PIText csv_text;
extern PIText save_dir_text;
extern PIText save_name_text;

#endif // __TEMPLATE_AUTOMATION_UI_H__
