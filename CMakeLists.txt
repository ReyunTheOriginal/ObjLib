cmake_minimum_required(VERSION 3.10)
project(ObjLib LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)  # For static libraries (.a, .lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)  # For shared libraries (.so, .dylib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/test) # For executables

# Find SDL2 packages
find_package(SDL2 REQUIRED)
find_library(SDL2_IMAGE_LIBRARY SDL2_image REQUIRED)
find_library(SDL2_TTF_LIBRARY SDL2_ttf REQUIRED)

# You'll need to manually find include directories
find_path(SDL2_IMAGE_INCLUDE_DIR SDL_image.h PATH_SUFFIXES SDL2)
find_path(SDL2_TTF_INCLUDE_DIR SDL_ttf.h PATH_SUFFIXES SDL2)

# Library target
file(GLOB_RECURSE OBJLIB_SRC "src/*.cpp")
add_library(ObjLib ${OBJLIB_SRC})
target_include_directories(ObjLib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIR}    # Changed from _INCLUDE_DIRS to _INCLUDE_DIR
    ${SDL2_TTF_INCLUDE_DIR}      # Changed from _INCLUDE_DIRS to _INCLUDE_DIR
)

add_custom_target(build_all ALL DEPENDS ObjLib)

# Link libraries to ObjLib since it uses SDL
target_link_libraries(ObjLib PUBLIC
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARY}        # Changed from _LIBRARIES to _LIBRARY
    ${SDL2_TTF_LIBRARY}          # Changed from _LIBRARIES to _LIBRARY
)

# Executable for testing
add_executable(ObjLibTest
    test/Test.cpp
)
target_link_libraries(ObjLibTest
    PRIVATE
        ObjLib
)