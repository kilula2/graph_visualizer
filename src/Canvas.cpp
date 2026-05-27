#include "Canvas.h"
#include "Vertex.h"
#include "Edge.h"
#include <QPainter>
#include <QPaintEvent>
#include <cmath>
#include <limits>

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
    if (!graph_ || graph_->getVertexCount() == 0) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Находим границы графа
    double minX = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();

    for (auto it = graph_->verticesBegin(); it != graph_->verticesEnd(); ++it) {
        minX = std::min(minX, it->getX());
        maxX = std::max(maxX, it->getX());
        minY = std::min(minY, it->getY());
        maxY = std::max(maxY, it->getY());
    }

    double margin = 50.0;
    minX -= margin; maxX += margin;
    minY -= margin; maxY += margin;

    double gWidth = maxX - minX;
    double gHeight = maxY - minY;
    if (gWidth < 1) gWidth = 1;
    if (gHeight < 1) gHeight = 1;

    double cw = width();
    double ch = height();
    double scale = std::min(cw / gWidth, ch / gHeight);

    double ox = (cw - gWidth * scale) / 2.0;
    double oy = (ch - gHeight * scale) / 2.0;

    const double vr = 20.0;

    for (auto it = graph_->edgesBegin(); it != graph_->edgesEnd(); ++it) {
        Vertex* v1 = it->getSource();
        Vertex* v2 = it->getTarget();
        bool dir = it->isDirected();
        bool loop = it->isLoop();

        double x1 = ox + (v1->getX() - minX) * scale;
        double y1 = oy + (v1->getY() - minY) * scale;
        double x2 = ox + (v2->getX() - minX) * scale;
        double y2 = oy + (v2->getY() - minY) * scale;

        painter.setPen(QPen(QColor(80, 80, 80), 2.0));

        if (loop) {
            painter.setBrush(Qt::NoBrush);
            double lx = x1 + vr * 0.7;
            double ly = y1 - vr * 1.2;
            double lr = vr * 0.8;
            painter.drawEllipse(QPointF(lx, ly), lr, lr);

            if (dir) {
                double ax = lx - lr * 0.6;
                double ay = ly + lr * 0.6;
                drawArrow(painter, QPointF(ax, ay), QPointF(ax + 10, ay - 10), 10);
            }
        } else {
            double dx = x2 - x1;
            double dy = y2 - y1;
            double dist = std::sqrt(dx * dx + dy * dy);
            if (dist > 0.001) {
                double nx = dx / dist;
                double ny = dy / dist;
                QPointF start(x1 + nx * vr, y1 + ny * vr);
                QPointF end(x2 - nx * vr, y2 - ny * vr);
                painter.drawLine(start, end);

                if (dir) {
                    QPointF from(end.x() - nx * 14, end.y() - ny * 14);
                    drawArrow(painter, end, from, 14);
                }
            }
        }

        if (it->getWeight() != 1.0) {
            std::string ws = std::to_string(it->getWeight());
            ws = ws.substr(0, ws.find('.') + 2);
            double mx, my;
            if (loop) { mx = x1 + vr * 0.7; my = y1 - vr * 2.5; }
            else { mx = (x1 + x2) / 2.0; my = (y1 + y2) / 2.0 - 10; }
            painter.setPen(QColor(160, 50, 50));
            painter.setFont(QFont("Arial", 10, QFont::Bold));
            painter.drawText(QPointF(mx, my), QString::fromStdString(ws));
        }
    }

    for (auto it = graph_->verticesBegin(); it != graph_->verticesEnd(); ++it) {
        double x = ox + (it->getX() - minX) * scale;
        double y = oy + (it->getY() - minY) * scale;

        painter.setBrush(Qt::white);
        painter.setPen(QPen(QColor(50, 50, 50), 2.0));
        painter.drawEllipse(QPointF(x, y), vr, vr);

        painter.setPen(Qt::black);
        painter.setFont(QFont("Segoe UI", 13, QFont::Bold));
        painter.drawText(QRectF(x - vr, y - vr, vr * 2, vr * 2),
                         Qt::AlignCenter,
                         QString::fromStdString(it->getLabel()));
    }
}

void Canvas::drawArrow(QPainter& painter, QPointF tip, QPointF from, double size) {
    QPointF dir = tip - from;
    double len = std::sqrt(dir.x() * dir.x() + dir.y() * dir.y());
    if (len < 0.001) return;
    QPointF n(-dir.y() / len, dir.x() / len);
    QPointF base = tip - (dir / len) * size;
    QPolygonF head;
    head << tip;
    head << base + n * size * 0.35;
    head << base - n * size * 0.35;
    painter.setBrush(QColor(80, 80, 80));
    painter.drawPolygon(head);
    painter.setBrush(Qt::NoBrush);
}
