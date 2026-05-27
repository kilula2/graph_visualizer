#include "Edge.h"
#include "Vertex.h"

Edge::Edge(Vertex* source, Vertex* target, bool directed, double weight)
    : source_(source)
    , target_(target)
    , directed_(directed)
    , weight_(weight)
{
}

Vertex* Edge::getSource() const {
    return source_;
}

Vertex* Edge::getTarget() const {
    return target_;
}

bool Edge::isDirected() const {
    return directed_;
}

bool Edge::isLoop() const {
    return source_ == target_;
}

double Edge::getWeight() const {
    return weight_;
}
