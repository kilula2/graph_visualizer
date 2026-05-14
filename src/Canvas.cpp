#include "Canvas.h"
#include "Vertex.h"
#include "Edge.h"
#include <QPainter>
#include <QPaintEvent>
#include <cmath>

Canvas::Canvas(QWidget* parent)
    : QWidget(parent)
    , graph_(nullptr)
{
    setStyleSheet("background-color: #ffffff; border: 1px solid #cccccc; border-radius: 6px;");
    setMinimumHeight(200);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void Canvas::setGraph(const Graph* graph) {
    graph_ = graph;
    update();
}

void Canvas::clearGraph() {
    graph_ = nullptr;
    update();
}

void Canvas::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);

    if (!graph_) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    double canvasWidth = width();
    double canvasHeight = height();
    double scaleX = canvasWidth / 800.0;
    double scaleY = canvasHeight / 600.0;
    double scale = std::min(scaleX, scaleY);

    double offsetX = (canvasWidth - 800.0 * scale) / 2.0;
    double offsetY = (canvasHeight - 600.0 * scale) / 2.0;

    const double vertexRadius = 20.0;

    // Рисуем рёбра
    painter.setPen(QPen(QColor(80, 80, 80), 2.0));
    for (auto it = graph_->edgesBegin(); it != graph_->edgesEnd(); ++it) {
        Vertex* v1 = it->getSource();
        Vertex* v2 = it->getTarget();

        double x1 = offsetX + v1->getX() * scale;
        double y1 = offsetY + (600.0 - v1->getY()) * scale;
        double x2 = offsetX + v2->getX() * scale;
        double y2 = offsetY + (600.0 - v2->getY()) * scale;

        // Рисуем линию от края до края круга
        double dx = x2 - x1;
        double dy = y2 - y1;
        double dist = std::sqrt(dx * dx + dy * dy);

        if (dist > 0.001) {
            double nx = dx / dist;
            double ny = dy / dist;

            double startX = x1 + nx * vertexRadius;
            double startY = y1 + ny * vertexRadius;
            double endX = x2 - nx * vertexRadius;
            double endY = y2 - ny * vertexRadius;

            painter.drawLine(QPointF(startX, startY), QPointF(endX, endY));
        }
    }

    // Рисуем вершины
    for (auto it = graph_->verticesBegin(); it != graph_->verticesEnd(); ++it) {
        double x = offsetX + it->getX() * scale;
        double y = offsetY + (600.0 - it->getY()) * scale;

        // Белый круг
        painter.setBrush(Qt::white);
        painter.setPen(QPen(QColor(50, 50, 50), 2.0));
        painter.drawEllipse(QPointF(x, y), vertexRadius, vertexRadius);

        // Метка в центре
        painter.setPen(Qt::black);
        QFont font("Arial", 14, QFont::Bold);
        painter.setFont(font);
        painter.drawText(QRectF(x - vertexRadius, y - vertexRadius,
                                vertexRadius * 2, vertexRadius * 2),
                         Qt::AlignCenter,
                         it->getLabel());
    }
}
