#include <gtest/gtest.h>
#include "Vertex.h"

TEST(VertexTest, Constructor) {
    Vertex v(0, "A");
    EXPECT_EQ(v.getId(), 0);
    EXPECT_EQ(v.getLabel(), "A");
}

TEST(VertexTest, Constructor_DifferentId) {
    Vertex v(5, "Node");
    EXPECT_EQ(v.getId(), 5);
    EXPECT_EQ(v.getLabel(), "Node");
}

TEST(VertexTest, Constructor_EmptyLabel) {
    Vertex v(0, "");
    EXPECT_EQ(v.getLabel(), "");
}

TEST(VertexTest, Position_Defaults) {
    Vertex v(0, "A");
    EXPECT_DOUBLE_EQ(v.getX(), 0.0);
    EXPECT_DOUBLE_EQ(v.getY(), 0.0);
}

TEST(VertexTest, Position_SetGet) {
    Vertex v(0, "A");
    v.setX(100.5);
    v.setY(200.3);
    EXPECT_DOUBLE_EQ(v.getX(), 100.5);
    EXPECT_DOUBLE_EQ(v.getY(), 200.3);
}

TEST(VertexTest, Displacement_Defaults) {
    Vertex v(0, "A");
    EXPECT_DOUBLE_EQ(v.getDx(), 0.0);
    EXPECT_DOUBLE_EQ(v.getDy(), 0.0);
}

TEST(VertexTest, Displacement_SetGet) {
    Vertex v(0, "A");
    v.setDx(5.5);
    v.setDy(-3.2);
    EXPECT_DOUBLE_EQ(v.getDx(), 5.5);
    EXPECT_DOUBLE_EQ(v.getDy(), -3.2);
}

TEST(VertexTest, Displacement_Reset) {
    Vertex v(0, "A");
    v.setDx(10.0);
    v.setDy(20.0);
    v.resetDisplacement();
    EXPECT_DOUBLE_EQ(v.getDx(), 0.0);
    EXPECT_DOUBLE_EQ(v.getDy(), 0.0);
}