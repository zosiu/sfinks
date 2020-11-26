set(APP_NAME "bird_lady_sfinks_example")
add_executable(${APP_NAME} main.cpp)

target_compile_definitions(${APP_NAME} PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")
target_compile_features(${APP_NAME} PRIVATE cxx_std_17)
if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(${APP_NAME} PRIVATE -Werror -Wall -Wextra -Wpedantic)
endif(CMAKE_COMPILER_IS_GNUCXX)

target_link_libraries(${APP_NAME} PRIVATE "sfinks")
target_link_libraries(${APP_NAME} PRIVATE "bird_lady")
