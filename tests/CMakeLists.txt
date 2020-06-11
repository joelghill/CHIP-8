find_package(Curses REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})
include_directories(. ../src)

add_executable(test_io test_io.cpp ../src/io.cpp)
add_executable(test_op_codes test_op_codes.cpp ../src/op_codes.cpp ../src/chip-8_state.cpp ../src/exceptions.cpp)
add_executable(test_display test_set_display.cpp ../src/op_codes.cpp ../src/chip-8_state.cpp ../src/exceptions.cpp ../src/display/terminal_display.cpp)

target_link_libraries(test_display ${CURSES_LIBRARIES})

add_test(NAME test_io COMMAND test_io WORKING_DIRECTORY ${UNIT_TEST_BIN_OUTPUT_DIR})
add_test(NAME test_op_codes COMMAND test_op_codes WORKING_DIRECTORY ${UNIT_TEST_BIN_OUTPUT_DIR})

