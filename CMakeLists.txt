cmake_minimum_required(VERSION 3.10)
project(ObjLib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/test)

# Find all SDL3 packages using find_package
find_package(SDL3 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3_IMAGE REQUIRED sdl3-image)
pkg_check_modules(SDL3_TTF REQUIRED sdl3-ttf)
pkg_check_modules(SDL3_MIXER REQUIRED sdl3-mixer)

# Library target
file(GLOB_RECURSE OBJLIB_SRC "src/*.cpp")
add_library(ObjLib ${OBJLIB_SRC})

target_include_directories(ObjLib PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Link using imported targets (cleaner and more reliable)
target_link_libraries(ObjLib PUBLIC
    SDL3::SDL3
    ${SDL3_IMAGE_LIBRARIES}
    ${SDL3_TTF_LIBRARIES}
    ${SDL3_MIXER_LIBRARIES}
)

target_include_directories(ObjLib PUBLIC ${SDL3_IMAGE_INCLUDE_DIRS} ${SDL3_TTF_INCLUDE_DIRS} ${SDL3_MIXER_INCLUDE_DIRS})

# Executable for testing
add_executable(ObjLibTest test/Test.cpp)
target_link_libraries(ObjLibTest PRIVATE ObjLib)