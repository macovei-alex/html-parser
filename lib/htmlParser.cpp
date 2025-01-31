#include "htmlParser.hpp"

#include "Token.hpp"

#include "xstd/string.hpp"

#include <iostream>
#include <format>


using namespace std::literals;


std::unique_ptr<hp::DocumentNode> hp::parseHtml(const std::string& html) {
	auto tokens = _private::tokenize(html);

	for (const auto& token : tokens) {
		std::cout << '(' << _private::getTokenName(token.type) << " -> \"" << token.value << "\")\n";
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
	{ TokenType::EqualSign, "EqualSign", R"(=)" },
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

	for (auto it = begin; it != end; ++it) {
		std::string tokenValue = it->str();
		for (const auto& tokenRegex : tokenRegexes) {
			if (std::regex_match(tokenValue, tokenRegex.regex)) {
				switch (tokenRegex.type) {
				case TokenType::WhiteSpace:
				case TokenType::Comment:
					break;
				case TokenType::TagName:
					if (tokenValue[1] == '/') {
						tokens.emplace_back(TokenType::TagEndOpen, "</");
						tokens.emplace_back(TokenType::TagName, tokenValue.substr(2));
					}
					else {
						tokens.emplace_back(TokenType::TagName, tokenValue.substr(1));
					}
					break;
				case TokenType::Text:
					tokens.emplace_back(TokenType::TagClose, ">");
					tokens.emplace_back(TokenType::Text,
						xstd::trim(tokenValue.substr(1), " \t\n"sv));
					break;
				case TokenType::AttributeValue:
					tokens.emplace_back(TokenType::EqualSign, "=");
					tokens.emplace_back(TokenType::AttributeValue, tokenValue.substr(1));
					break;
				default:
					tokens.emplace_back(tokenRegex.type, tokenValue);
					break;
				}

				break;
			}
		}
	}

	return tokens;
}
