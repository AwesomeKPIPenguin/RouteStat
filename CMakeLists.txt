
cmake_minimum_required(VERSION 3.10)

project(RouteStat)

set(CMAKE_CXX_STANDARD 14)

include_directories(inc json_lib/single_include/nlohmann)

add_executable(
        RouteStat
        src/main.cpp
        src/RoutePoint.cpp
        src/Point.cpp
        src/DB.cpp
        src/Polygon.cpp
        src/Connection.cpp
        src/Handlers.cpp
)

target_link_libraries(RouteStat zmq pthread pqxx pq)
