include_directories(.)

add_library(chip-8_lib chip-8.cpp io.cpp chip-8_state.cpp op_codes.cpp exceptions.cpp)

add_executable(chip-8 main.cpp chip-8.cpp io.cpp chip-8_state.cpp op_codes.cpp exceptions.cpp)