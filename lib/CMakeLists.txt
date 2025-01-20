add_library(html-parser
	"htmlParser.cpp"
	"htmlParser.hpp"
	"DocumentNode.cpp"
	"DocumentNode.hpp"
	"fileUtils.hpp"
	"fileUtils.cpp"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET html-parser PROPERTY CXX_STANDARD 20)
endif()
