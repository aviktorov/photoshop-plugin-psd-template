#ifndef __TEMPLATE_AUTIOMATION_H__
#define __TEMPLATE_AUTIOMATION_H__

#include "CSVData.h"
#include "SPInterf.h"
#include "TemplateAutomationTerminology.h"

extern SPPluginRef gPlugInRef;

int MakePSDTemplate(const char* name,const csv_row& header,const csv_row& data);

#endif // __TEMPLATE_AUTIOMATION_H__