cmake_minimum_required(VERSION 3.12)

project(mozz)

add_library(mozz SHARED dllmain.cpp
        include/debug.h
        include/debug.cpp
        include/include.h
        include/include.cpp
        include/print.h
        include/print.cpp
        include/injection/process_hallowing.h
        include/injection/process_hallowing.cpp
        include/injection/pe.h
        include/injection/pe.cpp
        include/injection/internal.h
)

set_target_properties(mozz PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        OUTPUT_NAME "mozz"
)

target_link_libraries(mozz PRIVATE wininet)