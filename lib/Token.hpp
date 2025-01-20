#pragma once

#include <regex>


namespace hp
{
	namespace _private
	{
		enum class TokenType : uint16_t {
			Unknown,
			TagOpen,
			TagClose,
			EqualSign,
			TagName,
			AttributeName,
			AttributeValue,
			Text,
			Comment,
			WhiteSpace,
		};

		struct Token {
			TokenType type;
			std::string value;
		};

		struct TokenRegex {
			TokenType type;
			std::string_view name;
			std::string_view regexStr;
			std::regex regex;

			TokenRegex(TokenType type, std::string_view name, std::string_view regexStr) {
				this->type = type;
				this->name = name;
				this->regexStr = regexStr;
				this->regex = std::regex(regexStr.data());
			}
		};
	}
}
