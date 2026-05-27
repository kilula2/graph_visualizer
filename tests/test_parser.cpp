#include <gtest/gtest.h>
#include "GraphParser.h"
#include "Graph.h"
#include <string>

// --- parse ---
TEST(ParserTest, Parse_SimpleUndirected) {
    GraphParser p;
    Graph g = p.parse("3\nA B C\nA B\nB C\n");
    EXPECT_EQ(g.getVertexCount(), 3);
    EXPECT_EQ(g.getEdgeCount(), 2);
}

TEST(ParserTest, Parse_DirectedArrow) {
    GraphParser p;
    Graph g = p.parse("2\nX Y\nX -> Y\n");
    EXPECT_TRUE(g.getEdges()[0].isDirected());
    EXPECT_EQ(g.getEdges()[0].getSource()->getLabel(), "X");
    EXPECT_EQ(g.getEdges()[0].getTarget()->getLabel(), "Y");
}

TEST(ParserTest, Parse_ReverseArrow) {
    GraphParser p;
    Graph g = p.parse("2\nX Y\nX <- Y\n");
    EXPECT_TRUE(g.getEdges()[0].isDirected());
    EXPECT_EQ(g.getEdges()[0].getSource()->getLabel(), "Y");
    EXPECT_EQ(g.getEdges()[0].getTarget()->getLabel(), "X");
}

TEST(ParserTest, Parse_DoubleArrow) {
    GraphParser p;
    Graph g = p.parse("2\nX Y\nX <-> Y\n");
    EXPECT_EQ(g.getEdgeCount(), 2);
    EXPECT_TRUE(g.getEdges()[0].isDirected());
    EXPECT_TRUE(g.getEdges()[1].isDirected());
}

TEST(ParserTest, Parse_WithWeight) {
    GraphParser p;
    Graph g = p.parse("2\nA B\nA -> B 5.5\n");
    EXPECT_DOUBLE_EQ(g.getEdges()[0].getWeight(), 5.5);
}

// --- parseVertexCount ---
TEST(ParserTest, VertexCount_Valid) {
    GraphParser p;
    Graph g = p.parse("5\nA B C D E\n");
    EXPECT_EQ(g.getVertexCount(), 5);
}

TEST(ParserTest, VertexCount_Zero) {
    GraphParser p;
    EXPECT_THROW(p.parse("0\nA\n"), std::runtime_error);
}

TEST(ParserTest, VertexCount_NotNumber) {
    GraphParser p;
    EXPECT_THROW(p.parse("abc\nA B\n"), std::runtime_error);
}

// --- parseLabels ---
TEST(ParserTest, Labels_Mismatch) {
    GraphParser p;
    EXPECT_THROW(p.parse("3\nA B\nA B\n"), std::runtime_error);
}

TEST(ParserTest, Labels_EmptyLine) {
    GraphParser p;
    EXPECT_THROW(p.parse("3\n\nA B\n"), std::runtime_error);
}

// --- parseEdge ---
TEST(ParserTest, Edge_NoOperator) {
    GraphParser p;
    Graph g = p.parse("2\nA B\nA B\n");
    EXPECT_EQ(g.getEdgeCount(), 1);
    EXPECT_FALSE(g.getEdges()[0].isDirected());
}

TEST(ParserTest, Edge_DashOperator) {
    GraphParser p;
    Graph g = p.parse("2\nA B\nA - B\n");
    EXPECT_EQ(g.getEdgeCount(), 1);
    EXPECT_FALSE(g.getEdges()[0].isDirected());
}

TEST(ParserTest, Edge_Loop) {
    GraphParser p;
    Graph g = p.parse("1\nA\nA A\n");
    EXPECT_TRUE(g.getEdges()[0].isLoop());
}

TEST(ParserTest, Edge_InvalidFormat) {
    GraphParser p;
    EXPECT_THROW(p.parse("2\nA B\nA\n"), std::runtime_error);
}
