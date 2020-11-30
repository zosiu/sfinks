cmake_minimum_required(VERSION 3.17)

project(
  sfinks_examples
  VERSION 1.0
  LANGUAGES CXX)

math(EXPR platform_bits "${CMAKE_SIZEOF_VOID_P} * 8")
set(platform_dir ${CMAKE_SYSTEM_NAME}${platform_bits})
foreach(config DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
  string(TOLOWER "${platform_dir}_${config}" outdir)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${config}
      "${CMAKE_SOURCE_DIR}/lib/${outdir}")
  set(CMAKE_ARCHIVE_LIBRARY_DIRECTORY_${config}
      "${CMAKE_SOURCE_DIR}/lib/${outdir}")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config}
      "${CMAKE_SOURCE_DIR}/bin/${outdir}")
endforeach()

set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
    ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(sfinks)
add_subdirectory(tic_tac_toe)
add_subdirectory(colors)
add_subdirectory(bird_lady)

add_subdirectory(examples/tic_tac_toe)
add_subdirectory(examples/colors)
add_subdirectory(examples/bird_lady)
