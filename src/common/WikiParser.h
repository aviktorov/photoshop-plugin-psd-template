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
		
		inline size_t getNumBoldTokens() const { return bold_tokens.size(); }
		inline size_t getNumItalicTokens() const { return italic_tokens.size(); }
		
		inline size_t getBoldStart(size_t num) const {
			assert(num >= 0 && num < bold_tokens.size() && "WikiParser::getBoldStart(): bad index");
			return bold_tokens[num].first;
		}
		
		inline size_t getBoldEnd(size_t num) const {
			assert(num >= 0 && num < bold_tokens.size() && "WikiParser::getBoldEnd(): bad index");
			return bold_tokens[num].second;
		}
		
		inline size_t getItalicStart(size_t num) const {
			assert(num >= 0 && num < bold_tokens.size() && "WikiParser::getItalicStart(): bad index");
			return italic_tokens[num].first;
		}
		
		inline size_t getItalicEnd(size_t num) const {
			assert(num >= 0 && num < bold_tokens.size() && "WikiParser::getItalicEnd(): bad index");
			return italic_tokens[num].second;
		}
		
	private:
		
		typedef std::pair<size_t,size_t> range;
		
		std::wstring processed_string;
		std::vector<range> bold_tokens;
		std::vector<range> italic_tokens;
};

#endif // __WIKI_PARSER_H__
