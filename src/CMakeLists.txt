include_directories(${CURSES_INCLUDE_DIR})

find_package(Curses REQUIRED)

add_library(chip-8_lib chip-8.cpp io.cpp chip-8_state.cpp op_codes.cpp exceptions.cpp)
add_executable(chip-8 main.cpp ./input/terminal_input.cpp ./display/terminal_display.cpp)


target_link_libraries(chip-8 ${CURSES_LIBRARIES})
target_link_libraries(chip-8 chip-8_lib)

install(TARGETS chip-8)
