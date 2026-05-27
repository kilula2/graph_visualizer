#ifndef EDGE_H
#define EDGE_H

class Vertex;

class Edge {
public:
    Edge(Vertex* source, Vertex* target, bool directed = false,
         double weight = 1.0);

    Vertex* getSource() const;
    Vertex* getTarget() const;

    bool isDirected() const;
    bool isLoop() const;

    double getWeight() const;

private:
    Vertex* source_;
    Vertex* target_;
    bool directed_;
    double weight_;
};

#endif // EDGE_H
