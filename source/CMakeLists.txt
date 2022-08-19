cmake_minimum_required (VERSION 3.8)

add_executable (jork "main.cpp" "texture.cpp" "parser.cpp" "console.cpp" "strings.cpp" "assets.ixx" "console.ixx" "constants.ixx" "parser.ixx" "strings.ixx" "texture.ixx" "world.ixx")
add_executable (parser_fuzzer "fuzzer_entry.cpp" "parser.cpp" "parser.ixx")

file(COPY ${PROJECT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})

target_compile_options(jork PRIVATE /std:c++latest "/experimental:logsarif\\")
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/sarif)
target_link_libraries(jork PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2::SDL2_ttf SDL2::SDL2_image)

target_compile_options(parser_fuzzer PRIVATE /std:c++latest /fsanitize=address /fsanitize=fuzzer /Zi)
target_link_options(parser_fuzzer PRIVATE /INCREMENTAL:NO)
