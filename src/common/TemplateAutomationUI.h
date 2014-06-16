#ifndef __TEMPLATE_AUTOMATION_UI_H__
#define __TEMPLATE_AUTOMATION_UI_H__

#include "PIUI.h"

enum {
	kDNoUI = -1,
	kDOk_button = 1,
	kDCancel_button = 2,
};

SPErr DoUI();
SPErr DoUIInit(PIDialogPtr dialog);

#endif // __TEMPLATE_AUTOMATION_UI_H__
