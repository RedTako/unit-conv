
add_executable(conversion_test conversion_test.cpp)
target_link_libraries(conversion_test lib)
add_test(NAME conversion_test COMMAND conversion_test)

add_executable(file_parse_test file_parse_test.cpp)
target_link_libraries(file_parse_test lib)
add_test(NAME file_parse_test COMMAND file_parse_test)
target_compile_definitions(file_parse_test PRIVATE "UNIT_FILE=\"${CMAKE_CURRENT_LIST_DIR}/../res/units.txt\"")

add_executable(regex_test regex_test.cpp)
target_link_libraries(regex_test lib)
add_test(NAME regex_test COMMAND regex_test)