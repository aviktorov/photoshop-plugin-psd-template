#ifndef __WIKI_PARSER_H__
#define __WIKI_PARSER_H__

#include <cassert>
#include <string>
#include <vector>

class WikiParser {
	public:
		
		WikiParser() { }
		~WikiParser() { }
		
		void parse(const std::wstring &data);
		
		inline const std::wstring &getProcessedString() const { return processed_string; }
		
		inline size_t getNumTokens() const { return tokens.size(); }
		
		inline size_t getTokenStart(size_t num) const {
			assert(num >= 0 && num < tokens.size() && "WikiParser::getTokenStart(): bad index");
			return tokens[num].start;
		}
		
		inline size_t getTokenEnd(size_t num) const {
			assert(num >= 0 && num < tokens.size() && "WikiParser::getTokenEnd(): bad index");
			return tokens[num].end;
		}
		
		inline bool getTokenBold(size_t num) const {
			assert(num >= 0 && num < tokens.size() && "WikiParser::getTokenBold(): bad index");
			return tokens[num].bold;
		}
		
		inline bool getTokenItalic(size_t num) const {
			assert(num >= 0 && num < tokens.size() && "WikiParser::getTokenItalic(): bad index");
			return tokens[num].italic;
		}
		
		inline size_t getNumParagraphs() const { return paragraphs.size(); }
		
		inline size_t getParagraphStart(size_t num) const {
			assert(num >= 0 && num < paragraphs.size() && "WikiParser::getParagraphStart(): bad index");
			return paragraphs[num].start;
		}
		
		inline size_t getParagraphEnd(size_t num) const {
			assert(num >= 0 && num < paragraphs.size() && "WikiParser::getParagraphEnd(): bad index");
			return paragraphs[num].end;
		}
		
	private:
		
		struct Token {
			size_t start;
			size_t end;
			bool bold;
			bool italic;
		};
		
		struct Paragraph {
			size_t start;
			size_t end;
		};
		
		std::wstring processed_string;
		std::vector<Token> tokens;
		std::vector<Paragraph> paragraphs;
};

#endif // __WIKI_PARSER_H__
