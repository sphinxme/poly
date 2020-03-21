cmake_minimum_required(VERSION 3.15)
project(poly)

set(CMAKE_CXX_STANDARD 17)


option(RELEASE "option for release" OFF)
if (RELEASE)
    set(CMAKE_EXE_LINKER_FLAGS -static)
    add_compile_options(-fexec-charset=gbk)
endif()

add_executable(polylist main.cpp PolyList.cpp PolyList.h Test.cpp Test.h)