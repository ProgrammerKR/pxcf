add_executable(test_pxcf unit/test_main.c unit/test_lexer.c unit/test_parser.c unit/test_value.c unit/test_serializer.c)
target_link_libraries(test_pxcf PRIVATE pxcf)
target_include_directories(test_pxcf PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_test(NAME unit_tests COMMAND test_pxcf)
