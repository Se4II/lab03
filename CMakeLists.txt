cmake_minimum_required(VERSION 3.4)
project(FormatterInc)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_subdirectory(formatter_lib)
add_subdirectory(formatter_ex_lib)
add_subdirectory(hello_world)
# add_subdirectory(solver)  # раскомментируй, когда solver_lib будет готов
