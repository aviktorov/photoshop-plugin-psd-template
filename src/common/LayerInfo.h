#ifndef __LAYER_INFO_H__
#define __LAYER_INFO_H__

#include "PIUI.h"

class LayerInfo {
	public:
		
		SPErr load(const std::wstring &name);
		SPErr save(PIActionDescriptor &data) const;
		
	private:
		
		struct TextStyle {
			std::string font_name;
			std::string font_style_name;
			float font_size;
			int horizontal_scale;
			int vertical_scale;
			int tracking;
		};
		
		struct ParagraphStyle {
			float start_indent;
			float end_indent;
			float first_line_indent;
			float space_before;
			float space_after;
			bool hyphenate;
		};
		
		TextStyle text;
		ParagraphStyle paragraph;
};


#endif // __LAYER_INFO_H__
