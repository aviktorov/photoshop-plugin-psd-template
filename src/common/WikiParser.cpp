#include "WikiParser.h"

static enum {
	TOKEN_CHAR,
	TOKEN_BOLD,
	TOKEN_ITALIC,
};

void WikiParser::parse(const std::wstring &data) {
	
	processed_string.reserve(data.size());
	processed_string = L"";
	
	int bold_opened = 0;
	int italic_opened = 0;
	
	for(size_t i = 0; i < data.size(); ++i) {
		int token_type = TOKEN_CHAR;
		
		if(data[i] == L'*') {
			token_type = TOKEN_ITALIC;
			
			if((i + 1) < data.size() && data[i + 1] == L'*') {
				token_type = TOKEN_BOLD;
				i++;
			}
		}
		
		switch(token_type) {
			case TOKEN_CHAR: {
				if(i < data.size()) {
					processed_string.push_back(data[i]);
				}
			} break;
			
			case TOKEN_BOLD: {
				if(bold_opened) {
					bold_tokens[bold_tokens.size() - 1].second = processed_string.size() - 1;
				}
				else {
					bold_tokens.push_back(range());
					bold_tokens[bold_tokens.size() - 1].first = processed_string.size();
				}
				bold_opened = !bold_opened;
			} break;
			
			case TOKEN_ITALIC: {
				if(italic_opened) {
					italic_tokens[italic_tokens.size() - 1].second = processed_string.size() - 1;
				}
				else {
					italic_tokens.push_back(range());
					italic_tokens[italic_tokens.size() - 1].first = processed_string.size();
				}
				italic_opened = !italic_opened;
			} break;
		};
	}
}
