cmake_minimum_required(VERSION 3.10)
project(Ashcore CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(raylib REQUIRED)
add_executable(Ashcore 
    main.cpp 
    player.cpp 
    world.cpp
    renderer.cpp
)
target_link_libraries(Ashcore raylib)
