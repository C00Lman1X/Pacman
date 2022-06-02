cmake_minimum_required(VERSION 3.0.0)
project(Pacman VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#include(CTest)
#enable_testing()

find_library(SDL2_LIB NAME SDL2 REQUIRED)
find_library(SDL2_IMAGE_LIB NAME SDL2_image REQUIRED)
find_library(SDL2_TTF_LIB NAME SDL2_ttf REQUIRED)
include_directories(
    /Library/Frameworks/SDL2.framework/Headers
    /Library/Frameworks/SDL2_image.framework/Headers
    /Library/Frameworks/SDL2_ttf.framework/Headers
)

add_executable(Pacman Pacman/main.cpp)
target_link_libraries(Pacman SDL2_LIB SDL2_IMAGE_LIB SDL2_TTF_LIB)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
