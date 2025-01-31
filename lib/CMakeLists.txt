add_library(html-parser
	"htmlParser.cpp"
	"htmlParser.hpp"
	"DocumentNode.cpp"
	"DocumentNode.hpp"
	"fileUtils.hpp"
	"fileUtils.cpp"
)

target_include_directories(html-parser PRIVATE "${CMAKE_SOURCE_DIR}")

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET html-parser PROPERTY CXX_STANDARD 20)
endif()
