cmake_minimum_required(VERSION 3.25)
project(Labortorio_2_GraficaC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(SDL2_INCLUDE_DIR "C:/Libs/include")
set(SDL2_LIB_DIR "C:/Libs/lib/x64")

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})


add_executable(Labortorio_2_GraficaC main.cpp)
target_link_libraries(Labortorio_2_GraficaC SDL2main SDL2)