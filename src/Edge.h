#ifndef EDGE_H
#define EDGE_H

class Vertex;

class Edge {
public:
    Edge(Vertex* source, Vertex* target);

    Vertex* getSource() const;
    Vertex* getTarget() const;

private:
    Vertex* source_;
    Vertex* target_;
};

#endif // EDGE_H
