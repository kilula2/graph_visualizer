#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include "Graph.h"

class Canvas : public QWidget {
    Q_OBJECT

public:
    explicit Canvas(QWidget* parent = nullptr);
    void setGraph(const Graph* graph);
    void clearGraph();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void drawArrow(QPainter& painter, QPointF tip, QPointF from, double size);

    const Graph* graph_;
};

#endif // CANVAS_H
