cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

set(CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

project(MusicPlayer VERSION 1.0 LANGUAGES C CXX)
include_directories(include)

#SFML
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR ${CMAKE_SOURCE_DIR}/external/SFML-2.5.1/lib/cmake/SFML)
find_package(SFML 2.5.1 COMPONENTS audio REQUIRED)

add_executable(MusicPlayer src/main.cpp src/trackList.cpp src/player.cpp)

target_link_libraries(MusicPlayer sfml-audio)

if(UNIX)
    target_link_libraries(MusicPlayer ${CMAKE_SOURCE_DIR}/external/libncurses.a)
endif(UNIX)
if(WIN32)
    target_link_libraries(MusicPlayer ${CMAKE_SOURCE_DIR}/external/pdcurses.lib)
endif(WIN32)