# Add source to this project's executable.
add_executable(tests
	"main.cpp"
)

target_include_directories(tests PRIVATE "${CMAKE_SOURCE_DIR}/lib")

target_link_libraries(tests PRIVATE html-parser)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET tests PROPERTY CXX_STANDARD 20)
endif()

set(RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources")
set(OUTPUT_DIR $<TARGET_FILE_DIR:tests>)

add_custom_command(
    TARGET tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${RESOURCES_DIR}" "${OUTPUT_DIR}/resources"
    COMMENT "Copying resources to output directory"
)
