#include "LayerInfo.h"

#include "PIUGet.h"
#include "PITerminology.h"
#include "Utils.h"

LayerInfo::LayerInfo() {
	text_style.font_name = "";
	text_style.font_size = 0.0f;
	text_style.font_size_units = typePointsUnit;
	text_style.font_style_name = "";
	text_style.horizontal_scale = 100;
	text_style.tracking = 0;
	text_style.vertical_scale = 100;
	
	paragraph_style.end_indent = 0.0f;
	paragraph_style.end_indent_units = typePointsUnit;
	paragraph_style.first_line_indent = 0.0f;
	paragraph_style.first_line_indent_units = typePointsUnit;
	paragraph_style.hyphenate = TRUE;
	paragraph_style.space_after = 0.0f;
	paragraph_style.space_after_units = typePointsUnit;
	paragraph_style.space_before = 0.0f;
	paragraph_style.space_before_units = typePointsUnit;
	paragraph_style.start_indent = 0.0f;
	paragraph_style.start_indent_units = typePointsUnit;
}

SPErr LayerInfo::load(const std::wstring &name) {
	
	// text layer
	Auto_Ref layer_ref;
	SPErr error = Utils::PutNameWString(layer_ref.get(),classLayer,name);
	if(error) return error;
	
	Auto_Desc layer;
	error = sPSActionControl->Get(&layer,layer_ref.get());
	if(error) return error;
	
	Auto_Desc text_layer;
	DescriptorClassID text_layer_id;
	error = sPSActionDescriptor->GetObjectA(layer.get(),keyText,&text_layer_id,&text_layer);
	if(error) return error;
	
	if(text_layer_id != classTextLayer) return kBadParameterErr;
	
	// lists
	Auto_List paragraph_list;
	error = PIUGetSingleItemFromDescriptor(text_layer.get(),keyParagraphStyleRange,&paragraph_list,NULL);
	if(error) return error;
	
	Auto_Desc paragraph_style_range;
	DescriptorClassID paragraph_id;
	error = sPSActionList->GetObjectA(paragraph_list.get(),0,&paragraph_id,&paragraph_style_range);
	if(error) return error;
	
	if(paragraph_id != keyParagraphStyleRange) return kBadParameterErr;
	
	Auto_Desc paragraph;
	error = sPSActionDescriptor->GetObjectA(paragraph_style_range.get(),keyParagraphStyle,&paragraph_id,&paragraph);
	if(error) return error;
	
	if(paragraph_id != keyParagraphStyle) return kBadParameterErr;
	
	Auto_List text_list;
	error = PIUGetSingleItemFromDescriptor(text_layer.get(),keyTextStyleRange,&text_list,NULL);
	if(error) return error;
	
	Auto_Desc text_style_range;
	DescriptorClassID text_id;
	error = sPSActionList->GetObjectA(text_list.get(),0,&text_id,&text_style_range);
	if(error) return error;
	
	if(text_id != keyTextStyleRange) return kBadParameterErr;
	
	Auto_Desc text;
	error = sPSActionDescriptor->GetObjectA(text_style_range.get(),keyTextStyle,&text_id,&text);
	if(error) return error;
	
	if(text_id != keyTextStyle) return kBadParameterErr;
	
	// text style
	uint32 font_name_size = 0;
	error = sPSActionDescriptor->GetStringLength(text.get(),keyFontName,&font_name_size);
	if(error) return error;
	
	font_name_size++;
	text_style.font_name = std::string(font_name_size,0);
	error = sPSActionDescriptor->GetString(text.get(),keyFontName,&text_style.font_name[0],font_name_size);
	if(error) return error;
	
	uint32 font_style_name_size = 0;
	error = sPSActionDescriptor->GetStringLength(text.get(),keyFontStyleName,&font_style_name_size);
	if(error) return error;
	
	font_style_name_size++;
	text_style.font_style_name = std::string(font_style_name_size,0);
	error = sPSActionDescriptor->GetString(text.get(),keyFontStyleName,&text_style.font_style_name[0],font_style_name_size);
	if(error) return error;
	
	error = sPSActionDescriptor->GetUnitFloat(text.get(),keySizeKey,&text_style.font_size_units,&text_style.font_size);
	if(error) return error;
	
	if(Utils::HasKey(text.get(),keyHorizontalScale)) {
		error = sPSActionDescriptor->GetInteger(text.get(),keyHorizontalScale,&text_style.horizontal_scale);
		if(error) return error;
	}
	
	if(Utils::HasKey(text.get(),keyVerticalScale)) {
		error = sPSActionDescriptor->GetInteger(text.get(),keyVerticalScale,&text_style.vertical_scale);
		if(error) return error;
	}
	
	if(Utils::HasKey(text.get(),keyTracking)) {
		error = sPSActionDescriptor->GetInteger(text.get(),keyTracking,&text_style.tracking);
		if(error) return error;
	}
	
	// paragraph style
	if(Utils::HasKey(paragraph.get(),keyAlignment)) {
		DescriptorEnumTypeID alignment_id;
		error = sPSActionDescriptor->GetEnumerated(paragraph.get(),keyAlignment,&alignment_id,&paragraph_style.alignment);
		if(error) return error;
		
		if(alignment_id != typeAlignment) return kBadParameterErr;
	}
	
	if(Utils::HasKey(paragraph.get(),keyFirstLineIndent)) {
		error = sPSActionDescriptor->GetUnitFloat(paragraph.get(),keyFirstLineIndent,&paragraph_style.first_line_indent_units,&paragraph_style.first_line_indent);
		if(error) return error;
	}
	
	if(Utils::HasKey(paragraph.get(),keyStartIndent)) {
		error = sPSActionDescriptor->GetUnitFloat(paragraph.get(),keyStartIndent,&paragraph_style.start_indent_units,&paragraph_style.start_indent);
		if(error) return error;
	}
	
	if(Utils::HasKey(paragraph.get(),keyEndIndent)) {
		error = sPSActionDescriptor->GetUnitFloat(paragraph.get(),keyEndIndent,&paragraph_style.end_indent_units,&paragraph_style.end_indent);
		if(error) return error;
	}
	
	if(Utils::HasKey(paragraph.get(),keySpaceBefore)) {
		error = sPSActionDescriptor->GetUnitFloat(paragraph.get(),keySpaceBefore,&paragraph_style.space_before_units,&paragraph_style.space_before);
		if(error) return error;
	}
	
	if(Utils::HasKey(paragraph.get(),keySpaceAfter)) {
		error = sPSActionDescriptor->GetUnitFloat(paragraph.get(),keySpaceAfter,&paragraph_style.space_after_units,&paragraph_style.space_after);
		if(error) return error;
	}
	
	if(Utils::HasKey(paragraph.get(),keyHyphenate)) {
		error = sPSActionDescriptor->GetBoolean(paragraph.get(),keyHyphenate,&paragraph_style.hyphenate);
		if(error) return error;
	}
	
	return error;
}

SPErr LayerInfo::saveTextStyle(PIActionDescriptor data) const {
	SPErr error = kNoErr;
	
	error = sPSActionDescriptor->PutString(data,keyFontName,text_style.font_name.c_str());
	if(error) return error;
	
	error = sPSActionDescriptor->PutString(data,keyFontStyleName,text_style.font_style_name.c_str());
	if(error) return error;
	
	error = sPSActionDescriptor->PutUnitFloat(data,keySizeKey,text_style.font_size_units,text_style.font_size);
	if(error) return error;
	
	error = sPSActionDescriptor->PutInteger(data,keyHorizontalScale,text_style.horizontal_scale);
	if(error) return error;
	
	error = sPSActionDescriptor->PutInteger(data,keyVerticalScale,text_style.vertical_scale);
	if(error) return error;
	
	error = sPSActionDescriptor->PutInteger(data,keyTracking,text_style.tracking);
	if(error) return error;
	
	return error;
}

SPErr LayerInfo::saveParagraphStyle(PIActionDescriptor data) const {
	SPErr error = kNoErr;
	
	error = sPSActionDescriptor->PutUnitFloat(data,keyFirstLineIndent,paragraph_style.first_line_indent_units,paragraph_style.first_line_indent);
	if(error) return error;
	
	error = sPSActionDescriptor->PutUnitFloat(data,keyStartIndent,paragraph_style.start_indent_units,paragraph_style.start_indent);
	if(error) return error;
	
	error = sPSActionDescriptor->PutUnitFloat(data,keyEndIndent,paragraph_style.end_indent_units,paragraph_style.end_indent);
	if(error) return error;
	
	error = sPSActionDescriptor->PutUnitFloat(data,keySpaceBefore,paragraph_style.space_before_units,paragraph_style.space_before);
	if(error) return error;
	
	error = sPSActionDescriptor->PutUnitFloat(data,keySpaceAfter,paragraph_style.space_after_units,paragraph_style.space_after);
	if(error) return error;
	
	error = sPSActionDescriptor->PutBoolean(data,keyHyphenate,paragraph_style.hyphenate);
	if(error) return error;
	
	error = sPSActionDescriptor->PutEnumerated(data,keyAlignment,typeAlignment,paragraph_style.alignment);
	if(error) return error;
	
	return error;
}