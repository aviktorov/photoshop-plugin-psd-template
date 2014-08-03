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
		
	private:
		
		struct Token {
			size_t start;
			size_t end;
			bool bold;
			bool italic;
		};
		
		std::wstring processed_string;
		std::vector<Token> tokens;
};

#endif // __WIKI_PARSER_H__
