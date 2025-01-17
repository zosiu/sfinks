cmake_minimum_required(VERSION 3.17)

project(
  sfinks
  VERSION 1.0
  LANGUAGES CXX)

set(APP_NAME "sfinks")

file(GLOB_RECURSE APP_SRCS src/*.cpp)
file(GLOB_RECURSE APP_HDRS include/sfinks/*.hpp)
file(GLOB_RECURSE APP_TPPS include/sfinks/*.tpp)
add_library(${APP_NAME} ${APP_SRCS} ${APP_HDRS} ${APP_TPPS})
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

# deps
set(LIB_DIR vendor)

set(CEREAL_DIR "${LIB_DIR}/cereal")
add_subdirectory(${CEREAL_DIR})
set(SKIP_PERFORMANCE_COMPARISON
    ON
    CACHE INTERNAL "")
set(JUST_INSTALL_CEREAL
    ON
    CACHE INTERNAL "")
target_link_libraries(${APP_NAME} PUBLIC "cereal")
