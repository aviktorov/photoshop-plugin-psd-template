#ifndef __TEMPLATE_AUTOMATION_UI_H__
#define __TEMPLATE_AUTOMATION_UI_H__

#include "PIUI.h"

enum {
	kDNoUI = -1,
	kDOk_button = 1,
	kDCancel_button = 2,
	kDCSV_edit = 6,
	kDChoose_button = 7,
	kDTemplate_edit = 8,
};

SPErr DoUI();
SPErr DoUIInit(PIDialogPtr handle);

extern PIText csv_text;
extern PIText template_text;

#endif // __TEMPLATE_AUTOMATION_UI_H__
