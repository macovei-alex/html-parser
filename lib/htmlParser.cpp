#include "htmlParser.hpp"

#include "xstd/string.hpp"

#include <iostream>
#include <format>


std::unique_ptr<hp::DocumentNode> hp::parseHtml(const std::string& html) {
	auto tokens = _private::tokenize(html);

	for (const auto& token : tokens) {
		std::cout << std::format("({}:{}:{}:{} -> \"{}\")\n",
			_private::getTokenName(token.type),
			token.contentPos,
			token.line,
			token.linePos,
			token.value);
	}

	auto root = std::make_unique<DocumentNode>();
	root->setParent(nullptr);
	root->setTag("html");

	return root;
}


const std::vector<hp::_private::TokenRegex> hp::_private::tokenRegexes = {
	{ TokenType::Comment, "Comment", R"(<!--(.*?)-->)" },
	{ TokenType::TagName, "TagName", R"((</?[\w]+)|(<!DOCTYPE))" },
	{ TokenType::Text, "Text", R"(>\s*[^\s<>][^<>]+\s*)" },
	{ TokenType::AttributeName, "AttributeName", R"([\w_-]+)" },
	{ TokenType::AttributeValue, "AttributeValue", R"(=("[^"]*")|('[^']*'))" },
	{ TokenType::TagClose, "TagClose", R"(/?>)" },
	{ TokenType::WhiteSpace, "WhiteSpace", R"(\s)" },
	{ TokenType::Unknown, "Unknown", R"(.)" },
	{ TokenType::TagStartOpen, "TagStartOpen", R"(<)" },
	{ TokenType::TagEndOpen, "TagEndOpen", R"(</)" },
};

std::string_view hp::_private::getTokenName(TokenType type) {
	auto it = std::find_if(tokenRegexes.begin(), tokenRegexes.end(),
		[type](const auto& tokenRegex) { return tokenRegex.type == type; });
	return it->name;
}

std::vector<hp::_private::Token> hp::_private::tokenize(const std::string& html) {
	std::string regexPattern = std::string(tokenRegexes[0].regexStr);
	for (size_t i = 1; i < tokenRegexes.size(); ++i) {
		regexPattern += "|";
		regexPattern += tokenRegexes[i].regexStr;
	}

	std::regex regex(regexPattern);
	std::vector<Token> tokens;

	auto begin = std::sregex_iterator(html.begin(), html.end(), regex);
	auto end = std::sregex_iterator();

	size_t contentPos = 1;
	size_t line = 1;
	size_t linePos = 1;

	for (auto it = begin; it != end; ++it) {
		std::string tokenValue = it->str();
		for (const auto& tokenRegex : tokenRegexes) {
			if (std::regex_match(tokenValue, tokenRegex.regex)) {
				switch (tokenRegex.type) {
				case TokenType::WhiteSpace:
				case TokenType::Comment:
					adjustTokenPositions(tokenValue, contentPos, line, linePos);
					break;
				case TokenType::TagName: {
					if (tokenValue[1] == '/') {
						tokens.emplace_back(TokenType::TagEndOpen, "</", contentPos, line, linePos);
						tokens.emplace_back(TokenType::TagName, tokenValue.substr(2), contentPos + 2, line, linePos + 2);
					}
					else {
						tokens.emplace_back(TokenType::TagName, tokenValue.substr(1), contentPos + 1, line, linePos + 1);
					}
					adjustTokenPositions(tokenValue, contentPos, line, linePos);
					break;
				}
				case TokenType::Text: {
					tokens.emplace_back(TokenType::TagClose, ">", contentPos, line, linePos);
					size_t tempContentPos = contentPos + 1;
					size_t tempLine = line;
					size_t tempLinePos = linePos + 1;
					for (size_t i = 1; i < tokenValue.size(); ++i) {
						if (tokenValue[i] == '\n') {
							++tempContentPos;
							++tempLine;
							tempLinePos = 1;
						}
						else if (tokenValue[i] != ' ' && tokenValue[i] != '\t') {
							break;
						}
						else {
							++tempContentPos;
							++tempLinePos;
						}
					}
					tokens.emplace_back(TokenType::Text, xstd::trim(tokenValue.substr(1), " \t\n"), tempContentPos, tempLine, tempLinePos);
					adjustTokenPositions(tokenValue, contentPos, line, linePos);
					break;
				}
				case TokenType::AttributeValue:
					tokens.emplace_back(TokenType::AttributeValue, tokenValue.substr(1), contentPos + 1, line, linePos + 1);
					adjustTokenPositions(tokenValue, contentPos, line, linePos);
					break;
				default:
					tokens.emplace_back(tokenRegex.type, tokenValue, contentPos, line, linePos);
					adjustTokenPositions(tokenValue, contentPos, line, linePos);
					break;
				}
				break;
			}
		}
	}

	return tokens;
}

void hp::_private::adjustTokenPositions(std::string_view tokenStr, size_t& contentPos, size_t& line, size_t& linePos) {
	contentPos += tokenStr.size();
	for (char c : tokenStr) {
		++linePos;
		if (c == '\n') {
			++line;
			linePos = 1;
		}
	}
}
