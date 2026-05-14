#include "SvgRenderer.h"
#include "Graph.h"
#include "Vertex.h"
#include "Edge.h"
#include "simple_svg.hpp"
#include <fstream>
#include <stdexcept>

void SvgRenderer::render(const Graph& graph, const QString& filename) {
    const int width = 800;
    const int height = 600;
    const int vertexRadius = 20;

    svg::Document doc(filename.toStdString(),
                      svg::Layout(svg::Dimensions(width, height),
                                  svg::Layout::BottomLeft));

    for (auto it = graph.verticesBegin(); it != graph.verticesEnd(); ++it) {
        double x = it->getX();
        double y = height - it->getY();

        doc << svg::Circle(svg::Point(x, y), vertexRadius,
                           svg::Fill(svg::Color(100, 149, 237)),
                           svg::Stroke(1.0, svg::Color(0, 0, 0)));

        doc << svg::Text(svg::Point(x, y + 5),
                         it->getLabel().toStdString(),
                         svg::Fill(svg::Color::Black),
                         svg::Font(14.0, "Arial"));
    }

    for (auto it = graph.edgesBegin(); it != graph.edgesEnd(); ++it) {
        double x1 = it->getSource()->getX();
        double y1 = height - it->getSource()->getY();
        double x2 = it->getTarget()->getX();
        double y2 = height - it->getTarget()->getY();

        doc << svg::Line(svg::Point(x1, y1), svg::Point(x2, y2),
                         svg::Stroke(1.5, svg::Color(50, 50, 50)));
    }

    if (!doc.save()) {
        throw std::runtime_error(
            "Не удалось сохранить файл: " + filename.toStdString());
    }
}
