cmake_minimum_required(VERSION 3.26)
project(PJC_Project)

set(CMAKE_CXX_STANDARD 20)

add_executable(PJC_Project main.cpp
)


set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML-2.6.1/lib/cmake/SFML)

find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(C:/SFML-2.6.1/include)
target_link_libraries(PJC_Project sfml-system sfml-window sfml-graphics sfml-audio)
