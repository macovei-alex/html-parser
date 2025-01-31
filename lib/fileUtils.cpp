#include "fileUtils.hpp"

#include <fstream>
#include <sstream>


std::string hp::readContents(std::istream& is) {
	std::stringstream buffer;
	buffer << is.rdbuf();
	return buffer.str();
}

std::string hp::readContents(std::string_view fileName) {
	std::ifstream fin(fileName.data());
	return readContents(fin);
}
