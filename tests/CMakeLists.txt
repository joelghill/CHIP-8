include_directories(. ../src)

add_executable(test_io test_io.cpp ../src/io.cpp)

add_test(NAME test_io COMMAND test_io WORKING_DIRECTORY ${UNIT_TEST_BIN_OUTPUT_DIR})