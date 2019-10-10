cmake_minimum_required(VERSION 3.5)
project(apng-exporter VERSION 0.1 LANGUAGES CXX)

if(MSVC OR MSYS OR MINGW)
    set(CMAKE_CXX_FLAGS "/std:c++17")
else()
    set(CMAKE_CXX_FLAGS "-fpermissive")
endif()

add_executable(apng-exporter src/main)
add_subdirectory("lib/fmt" EXCLUDE_FROM_ALL)

target_link_libraries(apng-exporter PRIVATE fmt::fmt-header-only)
