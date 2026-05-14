cmake_minimum_required(VERSION 3.16)
project(GraphVisualizer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCES
    src/main.cpp
    src/Vertex.cpp
    src/Edge.cpp
    src/Graph.cpp
    src/GraphParser.cpp
    src/ForceDirectedLayout.cpp
    src/SvgRenderer.cpp
    src/MainWindow.cpp
    src/Canvas.cpp
)

set(HEADERS
    src/Vertex.h
    src/Edge.h
    src/Graph.h
    src/GraphParser.h
    src/ForceDirectedLayout.h
    src/SvgRenderer.h
    src/MainWindow.h
    src/Canvas.h
    include/simple_svg.hpp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS}
    src/Canvas.h src/Canvas.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
