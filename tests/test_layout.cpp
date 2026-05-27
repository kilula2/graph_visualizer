#include <gtest/gtest.h>
#include "ForceDirectedLayout.h"
#include "Graph.h"
#include <cmath>

// --- run ---
TEST(LayoutTest, Run_EmptyGraph) {
    Graph g;
    auto& l = ForceDirectedLayout::getInstance();
    EXPECT_NO_THROW(l.run(g));
}

TEST(LayoutTest, Run_SingleVertex) {
    Graph g;
    g.addVertex("A");
    auto& l = ForceDirectedLayout::getInstance();
    EXPECT_NO_THROW(l.run(g));
    EXPECT_GE(g.getVertices()[0].getX(), 0);
    EXPECT_LE(g.getVertices()[0].getX(), 800);
}

TEST(LayoutTest, Run_Cycle) {
    Graph g;
    g.addVertex("A"); g.addVertex("B"); g.addVertex("C"); g.addVertex("D");
    g.addEdge("A","B"); g.addEdge("B","C"); g.addEdge("C","D"); g.addEdge("D","A");
    auto& l = ForceDirectedLayout::getInstance();
    EXPECT_NO_THROW(l.run(g));
}

TEST(LayoutTest, Run_Tree) {
    Graph g;
    g.addVertex("1"); g.addVertex("2"); g.addVertex("3"); g.addVertex("4"); g.addVertex("5");
    g.addEdge("1","2"); g.addEdge("1","3"); g.addEdge("2","4"); g.addEdge("2","5");
    auto& l = ForceDirectedLayout::getInstance();
    EXPECT_NO_THROW(l.run(g));
}

TEST(LayoutTest, Run_Complete) {
    Graph g;
    g.addVertex("A"); g.addVertex("B"); g.addVertex("C"); g.addVertex("D");
    g.addEdge("A","B"); g.addEdge("A","C"); g.addEdge("A","D");
    g.addEdge("B","C"); g.addEdge("B","D"); g.addEdge("C","D");
    auto& l = ForceDirectedLayout::getInstance();
    EXPECT_NO_THROW(l.run(g));
}

TEST(LayoutTest, Run_StarGraph) {
    Graph g;
    g.addVertex("Center");
    g.addVertex("A");
    g.addVertex("B");
    g.addVertex("C");
    g.addEdge("Center", "A");
    g.addEdge("Center", "B");
    g.addEdge("Center", "C");
    auto& l = ForceDirectedLayout::getInstance();
    EXPECT_NO_THROW(l.run(g));
}

// --- setParameters ---
TEST(LayoutTest, Params_Set) {
    auto& l = ForceDirectedLayout::getInstance();
    EXPECT_NO_THROW(l.setParameters(1000, 0.01, 50, 100));
}

TEST(LayoutTest, Params_Reset) {
    auto& l = ForceDirectedLayout::getInstance();
    l.setParameters(100, 0.1, 10, 50);
    EXPECT_NO_THROW(l.resetToDefaults());
}

// --- vertices position ---
TEST(LayoutTest, Positions_InBounds) {
    Graph g;
    for (char c = 'A'; c <= 'J'; ++c) {
        std::string label(1, c);
        g.addVertex(label);
    }
    g.addEdge("A","B"); g.addEdge("B","C"); g.addEdge("C","D"); g.addEdge("D","E");
    g.addEdge("E","F"); g.addEdge("F","G"); g.addEdge("G","H"); g.addEdge("H","I");
    g.addEdge("I","J"); g.addEdge("J","A");

    auto& l = ForceDirectedLayout::getInstance();
    l.run(g);

    for (auto it = g.verticesBegin(); it != g.verticesEnd(); ++it) {
        EXPECT_GE(it->getX(), 0.0);
        EXPECT_LE(it->getX(), 800.0);
        EXPECT_GE(it->getY(), 0.0);
        EXPECT_LE(it->getY(), 600.0);
    }
}

TEST(LayoutTest, Positions_NoOverlap) {
    Graph g;
    g.addVertex("A"); g.addVertex("B"); g.addVertex("C"); g.addVertex("D"); g.addVertex("E");
    g.addEdge("A","B"); g.addEdge("B","C"); g.addEdge("C","D"); g.addEdge("D","E");
    g.addEdge("E","A"); g.addEdge("A","C");

    auto& l = ForceDirectedLayout::getInstance();
    l.run(g);

    const auto& verts = g.getVertices();
    for (size_t i = 0; i < verts.size(); ++i) {
        for (size_t j = i + 1; j < verts.size(); ++j) {
            double dx = verts[i].getX() - verts[j].getX();
            double dy = verts[i].getY() - verts[j].getY();
            double dist = std::sqrt(dx * dx + dy * dy);
            EXPECT_GE(dist, 50.0) << "Overlap: " << verts[i].getLabel()
                                  << " and " << verts[j].getLabel();
        }
    }
}

// --- singleton ---
TEST(LayoutTest, Singleton_SameInstance) {
    auto& a = ForceDirectedLayout::getInstance();
    auto& b = ForceDirectedLayout::getInstance();
    EXPECT_EQ(&a, &b);
}
