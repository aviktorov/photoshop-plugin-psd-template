#ifndef __UTILS_H__
#define __UTILS_H__

#include "PIUI.h"

/*
 */
extern DescriptorKeyID keyBold;
extern DescriptorKeyID keyItalic;
extern DescriptorKeyID keyMaximizeCompatibility;
extern DescriptorKeyID keyParagraphStyle;
extern DescriptorKeyID keyParagraphStyleRange;

/*
 */
namespace Utils {
	
	SPErr Init();
	
	/*
	 */
	SPErr PutNameWString(PIActionReference ref,DescriptorClassID class_id,const std::wstring& str);
	SPErr PutWString(PIActionDescriptor desc,DescriptorKeyID key_id,const std::wstring& str);
}

#endif // __UTILS_H__
