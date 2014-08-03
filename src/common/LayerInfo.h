#ifndef __LAYER_INFO_H__
#define __LAYER_INFO_H__

#include "PIUI.h"

class LayerInfo {
	public:
		
		LayerInfo();
		
		SPErr load(const std::wstring &name);
		SPErr saveTextStyle(PIActionDescriptor data) const;
		SPErr saveParagraphStyle(PIActionDescriptor data) const;
		
	private:
		
		LayerInfo(const LayerInfo &info);
		LayerInfo &operator=(const LayerInfo &info);
		
		struct TextStyle {
			std::string font_name;
			std::string font_style_name;
			real64 font_size;
			DescriptorUnitID font_size_units;
			int32 horizontal_scale;
			int32 vertical_scale;
			int32 tracking;
		};
		
		struct ParagraphStyle {
			DescriptorUnitID start_indent_units;
			DescriptorUnitID end_indent_units;
			DescriptorUnitID first_line_indent_units;
			DescriptorUnitID space_before_units;
			DescriptorUnitID space_after_units;
			real64 start_indent;
			real64 end_indent;
			real64 first_line_indent;
			real64 space_before;
			real64 space_after;
			Boolean hyphenate;
		};
		
		TextStyle text_style;
		ParagraphStyle paragraph_style;
};


#endif // __LAYER_INFO_H__
