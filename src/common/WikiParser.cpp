#include "WikiParser.h"

static enum {
	TOKEN_CHAR,
	TOKEN_BOLD,
	TOKEN_ITALIC,
};

void WikiParser::parse(const std::wstring &data) {
	
	processed_string.reserve(data.size());
	processed_string = L"";
	
	Token current_token;
	int token_open = 0;
	current_token.start = 0;
	current_token.bold = false;
	current_token.italic = false;
	
	Paragraph current_paragraph;
	current_paragraph.start = 0;
	
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
					
					if(data[i] == L'\n') {
						current_paragraph.end = processed_string.size();
						if(current_paragraph.start != current_paragraph.end) paragraphs.push_back(current_paragraph);
						
						processed_string.push_back(L'\r');
					}
				}
			} break;
			
			case TOKEN_BOLD:
			case TOKEN_ITALIC: {
				current_token.end = processed_string.size();
				if(current_token.start != current_token.end) tokens.push_back(current_token);
				
				token_open = !token_open;
				
				current_token.start = processed_string.size();
				current_token.bold = token_open && (token_type == TOKEN_BOLD);
				current_token.italic = token_open && (token_type == TOKEN_ITALIC);
			} break;
		};
	}
	
	current_token.end = processed_string.size();
	if(current_token.start != current_token.end) tokens.push_back(current_token);
	
	current_paragraph.end = processed_string.size();
	if(current_paragraph.start != current_paragraph.end) paragraphs.push_back(current_paragraph);
}
