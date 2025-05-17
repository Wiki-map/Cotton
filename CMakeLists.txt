cmake_minimum_required(VERSION 3.10.0)
project(Cotton VERSION 0.1.0 LANGUAGES C)

file(GLOB_RECURSE MY_SOURCES CONFIG_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.c")

add_subdirectory("${CMAKE_SOURCE_DIR}\\third_party\\GLFW")
add_subdirectory("${CMAKE_SOURCE_DIR}\\third_party\\GLAD")

add_executable(Cotton "${MY_SOURCES}")

target_include_directories(Cotton PRIVATE "${CMAKE_SOURCE_DIR}/include/")
target_compile_definitions(Cotton PUBLIC RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/rez/")
target_link_libraries(Cotton glfw)
target_link_libraries(Cotton glad)
