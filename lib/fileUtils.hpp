#pragma once

#include <string>
#include <string_view>
#include <fstream>


namespace hp
{
	std::string readContents(std::istream& is);
	std::string readContents(std::string_view fileName);
}
