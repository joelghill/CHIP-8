include_directories(.)

add_library(chip-8 chip-8.cpp)
add_executable(main main.cpp chip-8.cpp)