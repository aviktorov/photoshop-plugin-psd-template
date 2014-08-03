#include "Utils.h"

/*
 */
DescriptorKeyID keyBold = 0;
DescriptorKeyID keyItalic = 0;
DescriptorKeyID keyMaximizeCompatibility = 0;
DescriptorKeyID keyParagraphStyle = 0;
DescriptorKeyID keyParagraphStyleRange = 0;
DescriptorKeyID keyFirstLineIndent = 0;
DescriptorKeyID keyStartIndent = 0;
DescriptorKeyID keyEndIndent = 0;
DescriptorKeyID keySpaceBefore = 0;
DescriptorKeyID keySpaceAfter = 0;
DescriptorKeyID keyHyphenate = 0;

DescriptorTypeID typePointsUnit = 0;

/*
 */
SPErr Utils::Init() {
	SPErr error = sPSActionControl->StringIDToTypeID("syntheticBold",&keyBold);
	if(error) return error;
	
	error = sPSActionControl->StringIDToTypeID("syntheticItalic",&keyItalic);
	if(error) return error;
	
	error = sPSActionControl->StringIDToTypeID("maximizeCompatibility",&keyMaximizeCompatibility);
	if(error) return error;
	
	error = sPSActionControl->StringIDToTypeID("paragraphStyle",&keyParagraphStyle);
	if(error) return error;
	
	error = sPSActionControl->StringIDToTypeID("paragraphStyleRange",&keyParagraphStyleRange);
	if(error) return error;
	
	error = sPSActionControl->StringIDToTypeID("firstLineIndent",&keyFirstLineIndent);
	if(error) return error;
	
	error = sPSActionControl->StringIDToTypeID("startIndent",&keyStartIndent);
	if(error) return error;
	
	error = sPSActionControl->StringIDToTypeID("endIndent",&keyEndIndent);
	if(error) return error;
	
	error = sPSActionControl->StringIDToTypeID("spaceBefore",&keySpaceBefore);
	if(error) return error;
	
	error = sPSActionControl->StringIDToTypeID("spaceAfter",&keySpaceAfter);
	if(error) return error;
	
	error = sPSActionControl->StringIDToTypeID("hyphenate",&keyHyphenate);
	if(error) return error;
	
	error = sPSActionControl->StringIDToTypeID("pointsUnit",&typePointsUnit);
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

SPErr Utils::PutWString(PIActionDescriptor desc,DescriptorKeyID key_id,const std::wstring& str) {
	ASZString temp_str;
	sASZString->MakeFromUnicode((ASUnicode*)&str[0],str.size(),&temp_str);
	return sPSActionDescriptor->PutZString(desc,key_id,temp_str);
}

/*
 */
bool Utils::HasKey(PIActionDescriptor desc,DescriptorKeyID key) {
	Boolean has_key = FALSE;
	
	SPErr error = sPSActionDescriptor->HasKey(desc,key,&has_key);
	if(error) return FALSE;
	
	return (has_key == TRUE);
}