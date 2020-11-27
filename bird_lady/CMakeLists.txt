cmake_minimum_required(VERSION 3.17)

project(
  bird_lady
  VERSION 1.0
  LANGUAGES CXX)

set(APP_NAME "bird_lady")
file(GLOB_RECURSE APP_SRCS src/*.cpp)
file(GLOB_RECURSE APP_HDRS include/bird_lady/*.hpp)
add_library(${APP_NAME} ${APP_SRCS} ${APP_HDRS})
target_include_directories(${APP_NAME}
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
    ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_compile_definitions(${APP_NAME} PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")
target_compile_features(${APP_NAME} PRIVATE cxx_std_17)
if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(${APP_NAME} PRIVATE -Werror -Wall -Wextra -Wpedantic)
endif(CMAKE_COMPILER_IS_GNUCXX)

set(Z3_LIB "vendor/z3")
add_subdirectory(${Z3_LIB})
set(Z3_BUILD_LIBZ3_SHARED
    OFF
    CACHE INTERNAL "")
target_link_libraries(${APP_NAME} PRIVATE "libz3")

target_link_libraries(${APP_NAME} PRIVATE sfinks)
