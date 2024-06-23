cmake_minimum_required (VERSION 3.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(kan)

add_executable(kan
    src/main.cpp
    src/asm.cpp
    src/bit.cpp
    src/file.cpp
    src/lexer.cpp
    src/parser.cpp
    src/rng.cpp
    src/str.cpp
    src/source.cpp
)

target_include_directories(kan 
    PRIVATE "${CMAKE_SOURCE_DIR}/include"
)

target_link_libraries(kan)