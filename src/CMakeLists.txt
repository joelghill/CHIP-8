include_directories(.)

add_library(chip-8_lib chip-8.cpp)

add_executable(chip-8 main.cpp chip-8.cpp io.cpp)