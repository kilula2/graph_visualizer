#include <gtest/gtest.h>
#include "Graph.h"
#include "Vertex.h"
#include "Edge.h"

// --- addVertex ---
TEST(GraphTest, AddVertex_One) {
    Graph g;
    g.addVertex("A");
    EXPECT_EQ(g.getVertexCount(), 1);
    EXPECT_EQ(g.getVertices()[0].getLabel(), "A");
}

TEST(GraphTest, AddVertex_Multiple) {
    Graph g;
    g.addVertex("A");
    g.addVertex("B");
    g.addVertex("C");
    EXPECT_EQ(g.getVertexCount(), 3);
}

TEST(GraphTest, AddVertex_DifferentLabels) {
    Graph g;
    g.addVertex("Node1");
    g.addVertex("Node2");
    EXPECT_EQ(g.getVertices()[0].getLabel(), "Node1");
    EXPECT_EQ(g.getVertices()[1].getLabel(), "Node2");
}

// --- addEdge (ненаправленное) ---
TEST(GraphTest, AddEdge_Undirected) {
    Graph g;
    g.addVertex("A");
    g.addVertex("B");
    g.addEdge("A", "B", false, 1.0);
    EXPECT_EQ(g.getEdgeCount(), 1);
    EXPECT_FALSE(g.getEdges()[0].isDirected());
}

TEST(GraphTest, AddEdge_Directed) {
    Graph g;
    g.addVertex("A");
    g.addVertex("B");
    g.addEdge("A", "B", true, 1.0);
    EXPECT_TRUE(g.getEdges()[0].isDirected());
    EXPECT_EQ(g.getEdges()[0].getSource()->getLabel(), "A");
    EXPECT_EQ(g.getEdges()[0].getTarget()->getLabel(), "B");
}

TEST(GraphTest, AddEdge_Loop) {
    Graph g;
    g.addVertex("A");
    g.addEdge("A", "A", false, 1.0);
    EXPECT_TRUE(g.getEdges()[0].isLoop());
}

TEST(GraphTest, AddEdge_Weight) {
    Graph g;
    g.addVertex("A");
    g.addVertex("B");
    g.addEdge("A", "B", false, 3.5);
    EXPECT_DOUBLE_EQ(g.getEdges()[0].getWeight(), 3.5);
}

TEST(GraphTest, AddEdge_ThrowsMissingSource) {
    Graph g;
    g.addVertex("A");
    EXPECT_THROW(g.addEdge("B", "A"), std::runtime_error);
}

TEST(GraphTest, AddEdge_ThrowsMissingTarget) {
    Graph g;
    g.addVertex("A");
    EXPECT_THROW(g.addEdge("A", "B"), std::runtime_error);
}

// --- findVertex ---
TEST(GraphTest, FindVertex_Exists) {
    Graph g;
    g.addVertex("Hello");
    Vertex* v = g.findVertex("Hello");
    ASSERT_NE(v, nullptr);
    EXPECT_EQ(v->getLabel(), "Hello");
}

TEST(GraphTest, FindVertex_NotExists) {
    Graph g;
    g.addVertex("A");
    EXPECT_EQ(g.findVertex("B"), nullptr);
}

TEST(GraphTest, FindVertex_Multiple) {
    Graph g;
    g.addVertex("X");
    g.addVertex("Y");
    g.addVertex("Z");
    Vertex* v = g.findVertex("Y");
    ASSERT_NE(v, nullptr);
    EXPECT_EQ(v->getLabel(), "Y");
}

// --- clear ---
TEST(GraphTest, Clear_RemovesAll) {
    Graph g;
    g.addVertex("A");
    g.addVertex("B");
    g.addEdge("A", "B");
    g.clear();
    EXPECT_EQ(g.getVertexCount(), 0);
    EXPECT_EQ(g.getEdgeCount(), 0);
}

TEST(GraphTest, Clear_EmptyGraph) {
    Graph g;
    g.clear();
    EXPECT_EQ(g.getVertexCount(), 0);
    EXPECT_EQ(g.getEdgeCount(), 0);
}

TEST(GraphTest, Clear_ThenAddAgain) {
    Graph g;
    g.addVertex("A");
    g.clear();
    g.addVertex("B");
    EXPECT_EQ(g.getVertexCount(), 1);
    EXPECT_EQ(g.getVertices()[0].getLabel(), "B");
}

// --- getVertexCount / getEdgeCount ---
TEST(GraphTest, Count_InitiallyZero) {
    Graph g;
    EXPECT_EQ(g.getVertexCount(), 0);
    EXPECT_EQ(g.getEdgeCount(), 0);
}

TEST(GraphTest, Count_AfterAdding) {
    Graph g;
    g.addVertex("A");
    g.addVertex("B");
    g.addEdge("A", "B");
    EXPECT_EQ(g.getVertexCount(), 2);
    EXPECT_EQ(g.getEdgeCount(), 1);
}

// --- iterators ---
TEST(GraphTest, Iterators_Empty) {
    Graph g;
    int count = 0;
    for (auto it = g.verticesBegin(); it != g.verticesEnd(); ++it) count++;
    EXPECT_EQ(count, 0);
}

TEST(GraphTest, Iterators_AllVertices) {
    Graph g;
    g.addVertex("A");
    g.addVertex("B");
    g.addVertex("C");
    int count = 0;
    for (auto it = g.verticesBegin(); it != g.verticesEnd(); ++it) count++;
    EXPECT_EQ(count, 3);
}

TEST(GraphTest, Iterators_Edges) {
    Graph g;
    g.addVertex("A");
    g.addVertex("B");
    g.addEdge("A", "B");
    g.addEdge("B", "A", true, 2.0);
    int count = 0;
    for (auto it = g.edgesBegin(); it != g.edgesEnd(); ++it) count++;
    EXPECT_EQ(count, 2);
}
