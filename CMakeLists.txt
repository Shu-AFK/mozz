cmake_minimum_required(VERSION 3.12)

project(mozz)

add_library(mozz SHARED dllmain.cpp
        include/process_doppeling.cpp
        include/process_doppeling.h
        include/debug.h
        include/debug.cpp
        include/include.h
        include/include.cpp
        include/print.h
        include/print.cpp
)

set_target_properties(mozz PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES)