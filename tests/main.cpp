#include "lib/htmlParser.hpp"
#include "lib/fileUtils.hpp"
#include "testFunctions.hpp"

#include <iostream>


int main()
{
	std::string html = hp::readContents("resources/test-tokenize-1.html");
	auto root = hp::parseHtml(html);
	
	hp::test::testTokenize(1);
	
	return 0;
}
