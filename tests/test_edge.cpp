#include <gtest/gtest.h>
#include "Edge.h"
#include "Vertex.h"

TEST(EdgeTest, Constructor_Undirected) {
    Vertex a(0, "A"), b(1, "B");
    Edge e(&a, &b, false, 1.0);
    EXPECT_FALSE(e.isDirected());
    EXPECT_EQ(e.getSource()->getLabel(), "A");
    EXPECT_EQ(e.getTarget()->getLabel(), "B");
}

TEST(EdgeTest, Constructor_Directed) {
    Vertex a(0, "A"), b(1, "B");
    Edge e(&a, &b, true, 1.0);
    EXPECT_TRUE(e.isDirected());
}

TEST(EdgeTest, Constructor_Weight) {
    Vertex a(0, "A"), b(1, "B");
    Edge e(&a, &b, false, 7.25);
    EXPECT_DOUBLE_EQ(e.getWeight(), 7.25);
}

TEST(EdgeTest, IsLoop_True) {
    Vertex a(0, "A");
    Edge e(&a, &a);
    EXPECT_TRUE(e.isLoop());
}

TEST(EdgeTest, IsLoop_False) {
    Vertex a(0, "A"), b(1, "B");
    Edge e(&a, &b);
    EXPECT_FALSE(e.isLoop());
}

TEST(EdgeTest, GetSource) {
    Vertex a(0, "X"), b(1, "Y");
    Edge e(&a, &b);
    EXPECT_EQ(e.getSource()->getId(), 0);
    EXPECT_EQ(e.getSource()->getLabel(), "X");
}

TEST(EdgeTest, GetTarget) {
    Vertex a(0, "X"), b(1, "Y");
    Edge e(&a, &b);
    EXPECT_EQ(e.getTarget()->getId(), 1);
    EXPECT_EQ(e.getTarget()->getLabel(), "Y");
}