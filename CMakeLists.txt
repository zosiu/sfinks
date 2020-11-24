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

set(TIC_TAC_TOE_APP_NAME "tic_tac_toe_sfinks_example")
add_executable(${TIC_TAC_TOE_APP_NAME} examples/tic_tac_toe/main.cpp)

target_compile_definitions(${TIC_TAC_TOE_APP_NAME}
                           PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")
target_compile_features(${TIC_TAC_TOE_APP_NAME} PRIVATE cxx_std_17)
if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(${TIC_TAC_TOE_APP_NAME} PRIVATE -Werror -Wall -Wextra
                                                         -Wpedantic)
endif(CMAKE_COMPILER_IS_GNUCXX)

# --- deps ---
add_subdirectory(sfinks)

add_subdirectory(tic_tac_toe)
target_link_libraries(${TIC_TAC_TOE_APP_NAME} PRIVATE "sfinks")
target_link_libraries(${TIC_TAC_TOE_APP_NAME} PRIVATE "tic_tac_toe")
