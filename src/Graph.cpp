#include "Graph.h"
#include <stdexcept>

void Graph::addVertex(const QString& label) {
    int id = static_cast<int>(vertices_.size());
    vertices_.emplace_back(id, label);
}

void Graph::addEdge(const QString& label1, const QString& label2) {
    Vertex* v1 = findVertex(label1);
    Vertex* v2 = findVertex(label2);

    if (!v1) {
        throw std::runtime_error(
            ("Вершина не найдена: " + label1).toStdString());
    }
    if (!v2) {
        throw std::runtime_error(
            ("Вершина не найдена: " + label2).toStdString());
    }

    edges_.emplace_back(v1, v2);
}

std::vector<Vertex>& Graph::getVertices() {
    return vertices_;
}

const std::vector<Vertex>& Graph::getVertices() const {
    return vertices_;
}

const std::vector<Edge>& Graph::getEdges() const {
    return edges_;
}

int Graph::getVertexCount() const {
    return static_cast<int>(vertices_.size());
}

int Graph::getEdgeCount() const {
    return static_cast<int>(edges_.size());
}

Vertex* Graph::findVertex(const QString& label) {
    for (auto& v : vertices_) {
        if (v.getLabel() == label) {
            return &v;
        }
    }
    return nullptr;
}

Graph::ConstVertexIterator Graph::verticesBegin() const {
    return vertices_.cbegin();
}

Graph::ConstVertexIterator Graph::verticesEnd() const {
    return vertices_.cend();
}

Graph::VertexIterator Graph::verticesBegin() {
    return vertices_.begin();
}

Graph::VertexIterator Graph::verticesEnd() {
    return vertices_.end();
}

Graph::ConstEdgeIterator Graph::edgesBegin() const {
    return edges_.cbegin();
}

Graph::ConstEdgeIterator Graph::edgesEnd() const {
    return edges_.cend();
}

void Graph::clear() {
    vertices_.clear();
    edges_.clear();
}
