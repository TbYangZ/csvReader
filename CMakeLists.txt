cmake_minimum_required(VERSION 3.10)
project(csvReader)
set(CMAKE_CXX_STANDARD 23)
include_directories(./include)
add_executable(main
    ./src/csvReader.cpp
    main.cpp
)