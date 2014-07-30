#include "Utils.h"

/*
 */
DescriptorKeyID keyBold = 0;
DescriptorKeyID keyItalic = 0;
DescriptorTypeID keyMaximizeCompatibility = 0;

/*
 */
SPErr Utils::Init() {
	SPErr error = sPSActionControl->StringIDToTypeID("syntheticBold",&keyBold);
	if(error) return error;
	
	error = sPSActionControl->StringIDToTypeID("syntheticItalic",&keyItalic);
	if(error) return error;
	
	error = sPSActionControl->StringIDToTypeID("maximizeCompatibility",&keyMaximizeCompatibility);
	if(error) return error;
	
	return error;
}

/*
 */
SPErr Utils::PutNameWString(PIActionReference ref,DescriptorClassID class_id,const std::wstring& str) {
	ASZString temp_str;
	sASZString->MakeFromUnicode((ASUnicode*)&str[0],str.size(),&temp_str);
	return sPSActionReference->PutNameZString(ref,class_id,temp_str);
}

/*
 */
SPErr Utils::PutWString(PIActionDescriptor desc,DescriptorKeyID key_id,const std::wstring& str) {
	ASZString temp_str;
	sASZString->MakeFromUnicode((ASUnicode*)&str[0],str.size(),&temp_str);
	return sPSActionDescriptor->PutZString(desc,key_id,temp_str);
}
