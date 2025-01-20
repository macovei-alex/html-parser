#include "fileUtils.hpp"

#include <fstream>
#include <sstream>


std::string hp::readFileContents(std::string_view fileName) {
	std::stringstream buffer;
	std::ifstream fin(fileName.data());
	buffer << fin.rdbuf();
	return buffer.str();
}
