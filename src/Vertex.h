#ifndef VERTEX_H
#define VERTEX_H

#include <string>

class Vertex {
public:
    Vertex(int id, const std::string& label);

    int getId() const;
    std::string getLabel() const;

    double getX() const;
    double getY() const;
    void setX(double x);
    void setY(double y);

    double getDx() const;
    double getDy() const;
    void setDx(double dx);
    void setDy(double dy);
    void resetDisplacement();

private:
    int id_;
    std::string label_;
    double x_;
    double y_;
    double dx_;
    double dy_;
};

#endif // VERTEX_H
