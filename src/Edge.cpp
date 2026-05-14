#include "Edge.h"
#include "Vertex.h"

Edge::Edge(Vertex* source, Vertex* target)
    : source_(source)
    , target_(target)
{
}

Vertex* Edge::getSource() const {
    return source_;
}

Vertex* Edge::getTarget() const {
    return target_;
}
