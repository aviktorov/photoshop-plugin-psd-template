#ifndef __TEMPLATE_AUTIOMATION_H__
#define __TEMPLATE_AUTIOMATION_H__

#include "SPInterf.h"
#include "TemplateAutomationTerminology.h"

#include <string>
#include <vector>

extern SPPluginRef gPlugInRef;
typedef std::vector<std::string> csv_row;

int MakePSDTemplate(const char* name,const csv_row& header,const csv_row& data);

#endif // __TEMPLATE_AUTIOMATION_H__