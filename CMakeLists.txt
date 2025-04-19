cmake_minimum_required(VERSION 3.15)

project(MyProject VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Apply -ferror-limit=0 if using Clang
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-ferror-limit=0)
endif()

# Add source files
add_executable(MyProject
        main.cpp
)
