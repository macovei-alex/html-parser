#pragma once

#include "DocumentNode.hpp"
#include "Token.hpp"


namespace hp
{
	std::unique_ptr<hp::DocumentNode> parseHtml(const std::string& html);


	namespace _private
	{
		extern const std::vector<TokenRegex> tokenRegexes;

		std::string_view getTokenName(TokenType type);
		std::vector<Token> tokenize(const std::string& html);
	}
}
