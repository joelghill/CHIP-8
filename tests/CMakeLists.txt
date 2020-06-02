include_directories(. ../src)

add_executable(test_io test_io.cpp ../src/io.cpp)
add_executable(test_set_display test_set_display.cpp ../src/chip-8_state.cpp ../src/op_codes.cpp ../src/exceptions.cpp)


add_test(NAME test_io COMMAND test_io WORKING_DIRECTORY ${UNIT_TEST_BIN_OUTPUT_DIR})
add_test(NAME test_set_display COMMAND test_set_display WORKING_DIRECTORY ${UNIT_TEST_BIN_OUTPUT_DIR})
