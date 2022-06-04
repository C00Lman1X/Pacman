cmake_minimum_required(VERSION 3.20.0)
project(Pacman VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_library(SDL2_LIB NAME SDL2 PATHS /usr/local/lib REQUIRED)
find_library(SDL2_IMAGE_LIB NAME SDL2_image PATHS /usr/local/lib REQUIRED)
find_library(SDL2_TTF_LIB NAME SDL2_ttf PATHS /usr/local/lib REQUIRED)

include_directories(
    /usr/local/include/SDL2
)

file(GLOB_RECURSE PACMAN_SOURCES Pacman/*.cpp)
file(GLOB_RECURSE PACMAN_HEADERS Pacman/*.h)

file(GLOB DATA_PNG data/*.png)
file(COPY ${DATA_PNG} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY data/map.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(GLOB DATA_FONTS data/freefont-ttf/sfd/*.*)
file(COPY ${DATA_FONTS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/freefont-ttf/sfd)

add_executable(Pacman ${PACMAN_SOURCES})
target_link_libraries(Pacman ${SDL2_LIB} ${SDL2_IMAGE_LIB} ${SDL2_TTF_LIB})