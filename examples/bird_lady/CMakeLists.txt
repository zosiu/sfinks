set(APP_NAME_2P "bird_lady_sfinks_example_2p")
add_executable(${APP_NAME_2P} main_2p.cpp)

target_compile_definitions(${APP_NAME_2P} PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")
target_compile_features(${APP_NAME_2P} PRIVATE cxx_std_17)
if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(${APP_NAME_2P} PRIVATE -Werror -Wall -Wextra
                                                -Wpedantic)
endif(CMAKE_COMPILER_IS_GNUCXX)

target_link_libraries(${APP_NAME_2P} PRIVATE "sfinks")
target_link_libraries(${APP_NAME_2P} PRIVATE "bird_lady")

# --

set(APP_NAME_3P "bird_lady_sfinks_example_3p")
add_executable(${APP_NAME_3P} main_3p.cpp)

target_compile_definitions(${APP_NAME_3P} PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")
target_compile_features(${APP_NAME_3P} PRIVATE cxx_std_17)
if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(${APP_NAME_3P} PRIVATE -Werror -Wall -Wextra
                                                -Wpedantic)
endif(CMAKE_COMPILER_IS_GNUCXX)

target_link_libraries(${APP_NAME_3P} PRIVATE "sfinks")
target_link_libraries(${APP_NAME_3P} PRIVATE "bird_lady")
