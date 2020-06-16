include_directories(${CURSES_INCLUDE_DIR})
include_directories(${SDL2_INCLUDE_DIR})

find_package(Curses REQUIRED)
find_package(SDL2 REQUIRED)

add_library(chip-8_lib chip-8.cpp io.cpp chip-8_state.cpp op_codes.cpp exceptions.cpp)
add_executable(chip-8 main.cpp ./input/sdl2_input.cpp ./display/terminal_display.cpp ./display/mock_display.cpp)


target_link_libraries(chip-8 ${CURSES_LIBRARIES})
target_link_libraries(chip-8 chip-8_lib)
target_link_libraries(chip-8 ${SDL2_LIBRARY})
