#pragma once

#include <iostream>

#include "lib/Token.hpp"


namespace hp
{
	namespace test
	{
		void testTokenize(int testId, std::ostream& os = std::cout);


		namespace _private
		{
			std::vector<hp::_private::Token> parseExpectContent(std::string& content);
		}
	}
}
