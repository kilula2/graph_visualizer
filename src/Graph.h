#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include "Vertex.h"
#include "Edge.h"

class Graph {
public:
    void addVertex(const QString& label);
    void addEdge(const QString& label1, const QString& label2);

    std::vector<Vertex>& getVertices();
    const std::vector<Vertex>& getVertices() const;
    const std::vector<Edge>& getEdges() const;

    int getVertexCount() const;
    int getEdgeCount() const;

    Vertex* findVertex(const QString& label);

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
