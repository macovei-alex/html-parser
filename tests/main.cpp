#include "htmlParser.hpp"
#include "fileUtils.hpp"

#include <iostream>


int main()
{
	std::string html = hp::readFileContents("resources/test.html");
	auto root = hp::parseHtml(html);
	// std::cout << root->getHierarchyRepr();
	return 0;
}
