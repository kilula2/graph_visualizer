#ifndef VERTEX_H
#define VERTEX_H

#include <QString>

class Vertex {
public:
    Vertex(int id, const QString& label);

    int getId() const;
    QString getLabel() const;

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
    QString label_;
    double x_;
    double y_;
    double dx_;
    double dy_;
};

#endif // VERTEX_H
