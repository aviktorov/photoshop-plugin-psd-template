#ifndef __TEMPLATE_AUTIOMATION_H__
#define __TEMPLATE_AUTIOMATION_H__

#include "SPInterf.h"
#include "TemplateAutomationTerminology.h"

#include <string>

extern SPPluginRef gPlugInRef;

SPErr SelectLayerByName(const std::wstring& text);
SPErr SetCurrentLayerText(const std::wstring& text);
SPErr SavePSD(const char* name);

#endif // __TEMPLATE_AUTIOMATION_H__