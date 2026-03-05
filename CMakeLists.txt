cmake_minimum_required(VERSION 3.20)
project (gbemulator C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS_RELEASE "-O2")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
file(GLOB_RECURSE src_files
        src/*.c
        include/*.h
)

add_executable(gbemulator ${src_files})
target_include_directories(gbemulator PRIVATE include)

find_package(SDL3 REQUIRED)
target_include_directories(gbemulator PRIVATE ${SDL3_INCLUDE_DIRS})
target_link_libraries(gbemulator PRIVATE ${SDL3_LIBRARIES})
