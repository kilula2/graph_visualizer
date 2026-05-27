#include "Vertex.h"

Vertex::Vertex(int id, const std::string& label)
    : id_(id)
    , label_(label)
    , x_(0.0)
    , y_(0.0)
    , dx_(0.0)
    , dy_(0.0)
{
}

int Vertex::getId() const {
    return id_;
}

std::string Vertex::getLabel() const {
    return label_;
}

double Vertex::getX() const {
    return x_;
}

double Vertex::getY() const {
    return y_;
}

void Vertex::setX(double x) {
    x_ = x;
}

void Vertex::setY(double y) {
    y_ = y;
}

double Vertex::getDx() const {
    return dx_;
}

double Vertex::getDy() const {
    return dy_;
}

void Vertex::setDx(double dx) {
    dx_ = dx;
}

void Vertex::setDy(double dy) {
    dy_ = dy;
}

void Vertex::resetDisplacement() {
    dx_ = 0.0;
    dy_ = 0.0;
}
