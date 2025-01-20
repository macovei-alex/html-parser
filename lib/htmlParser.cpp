#include "htmlParser.hpp"

#include "Token.hpp"

#include <iostream>
#include <format>


namespace hp::_private
{
	extern const std::vector<TokenRegex> tokenRegexes;

	static inline std::string_view getTokenName(TokenType type);
	static std::vector<Token> tokenize(const std::string& html);
}



std::unique_ptr<hp::DocumentNode> hp::parseHtml(const std::string& html) {
	auto tokens = _private::tokenize(html);

	for (const auto& token : tokens) {
		std::cout << '(' << _private::getTokenName(token.type) << " -> " << token.value << ")\n";
	}

	auto root = std::make_unique<DocumentNode>();
	root->setParent(nullptr);
	root->setTag("html");

	return root;
}



static const std::vector<hp::_private::TokenRegex> hp::_private::tokenRegexes = {
	{ TokenType::Comment, "Comment", R"(<!--(.*?)-->)" },
	{ TokenType::TagName, "TagName", R"(<[!?\w]+)" },
	{ TokenType::Text, "Text", R"(>\s*[^\s<>][^<>]+\s*</)" },
	{ TokenType::AttributeName, "AttributeName", R"([\w_-]+)" },
	{ TokenType::AttributeValue, "AttributeValue", R"(=("[^"]*")|('[^']*'))" },
	{ TokenType::TagOpen, "TagOpen", R"(</?)" },
	{ TokenType::TagClose, "TagClose", R"(/?>)" },
	{ TokenType::EqualSign, "EqualSign", R"(=)" },
	{ TokenType::WhiteSpace, "WhiteSpace", R"(\s)" },
	{ TokenType::Unknown, "Unknown", R"(.)" },
};

static inline std::string_view hp::_private::getTokenName(TokenType type) {
	auto it = std::find_if(tokenRegexes.begin(), tokenRegexes.end(),
		[type](const auto& tokenRegex) { return tokenRegex.type == type; });
	return it->name;
}

static inline std::vector<hp::_private::Token> hp::_private::tokenize(const std::string& html) {
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
					break;
				case TokenType::TagName:
					tokens.emplace_back(TokenType::TagOpen, "<");
					tokens.emplace_back(tokenRegex.type, tokenValue.substr(1));
					break;
				case TokenType::Text:
					tokens.emplace_back(TokenType::TagClose, ">");
					tokens.emplace_back(tokenRegex.type, tokenValue.substr(1, tokenValue.size() - 3));
					tokens.emplace_back(TokenType::TagOpen, "</");
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
