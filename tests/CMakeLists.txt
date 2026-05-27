find_package(GTest QUIET)
if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    FetchContent_MakeAvailable(googletest)
endif()

set(TEST_SOURCES
    test_vertex.cpp
    test_edge.cpp
    test_graph.cpp
    test_parser.cpp
    test_layout.cpp
    ../src/Vertex.cpp
    ../src/Edge.cpp
    ../src/Graph.cpp
    ../src/GraphParser.cpp
    ../src/ForceDirectedLayout.cpp
)

add_executable(graph_tests ${TEST_SOURCES})

target_include_directories(graph_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(graph_tests PRIVATE GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(graph_tests)