#pragma once

#include <regex>


namespace hp::_private
{
	enum class TokenType : uint8_t {
		Unknown,
		TagStartOpen,
		TagEndOpen,
		TagClose,
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
		size_t contentPos;
		size_t line;
		size_t linePos;

		bool operator==(const Token& other) const {
			return type == other.type
				&& value == other.value
				&& contentPos == other.contentPos
				&& line == other.line
				&& linePos == other.linePos;
		}
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
