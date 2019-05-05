cmake_minimum_required(VERSION 3.10)
project(RouteStat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

include_directories(inc inc/boo json_lib/single_include/nlohmann)

add_executable(RouteStat
        src/main.cpp
        src/RoutePoint.cpp
        src/Point.cpp
        inc/Point.hpp
        src/DB.cpp
        inc/DB.hpp
        src/Polygon.cpp
        inc/Polygon.hpp
        src/test.cpp
        )