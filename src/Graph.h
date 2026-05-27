#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include "Vertex.h"
#include "Edge.h"

class Graph {
public:
    void addVertex(const std::string& label);

    void addEdge(const std::string& label1, const std::string& label2,
                 bool directed = false, double weight = 1.0);

    std::vector<Vertex>& getVertices();
    const std::vector<Vertex>& getVertices() const;
    const std::vector<Edge>& getEdges() const;

    int getVertexCount() const;
    int getEdgeCount() const;

    Vertex* findVertex(const std::string& label);

    using VertexIterator = std::vector<Vertex>::iterator;
    using ConstVertexIterator = std::vector<Vertex>::const_iterator;
    using EdgeIterator = std::vector<Edge>::iterator;
    using ConstEdgeIterator = std::vector<Edge>::const_iterator;

    ConstVertexIterator verticesBegin() const;
    ConstVertexIterator verticesEnd() const;
    VertexIterator verticesBegin();
    VertexIterator verticesEnd();
    ConstEdgeIterator edgesBegin() const;
    ConstEdgeIterator edgesEnd() const;

    void clear();

private:
    std::vector<Vertex> vertices_;
    std::vector<Edge> edges_;
};

#endif // GRAPH_H
