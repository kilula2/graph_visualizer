#include "SvgRenderer.h"
#include "Graph.h"
#include "Vertex.h"
#include "Edge.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <stdexcept>
#include <limits>

void SvgRenderer::render(const Graph& graph, const std::string& filename) {
    if (graph.getVertexCount() == 0) return;

    const int VR = 20;
    double margin = 50.0;

    // Находим границы
    double minX = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::lowest();

    for (auto it = graph.verticesBegin(); it != graph.verticesEnd(); ++it) {
        minX = std::min(minX, it->getX());
        maxX = std::max(maxX, it->getX());
        minY = std::min(minY, it->getY());
        maxY = std::max(maxY, it->getY());
    }

    minX -= margin; maxX += margin;
    minY -= margin; maxY += margin;

    int W = static_cast<int>(std::ceil(maxX - minX));
    int H = static_cast<int>(std::ceil(maxY - minY));
    if (W < 100) W = 800;
    if (H < 100) H = 600;

    std::stringstream ss;
    ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
       << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << W
       << "\" height=\"" << H << "\">\n"
       << "<defs>\n"
       << "<marker id=\"arrow\" markerWidth=\"10\" markerHeight=\"7\" "
       << "refX=\"10\" refY=\"3.5\" orient=\"auto\">\n"
       << "<polygon points=\"0 0, 10 3.5, 0 7\" fill=\"#505050\"/>\n"
       << "</marker>\n"
       << "</defs>\n"
       << "<rect width=\"100%\" height=\"100%\" fill=\"#ffffff\"/>\n";

    for (auto it = graph.edgesBegin(); it != graph.edgesEnd(); ++it) {
        double x1 = it->getSource()->getX() - minX;
        double y1 = it->getSource()->getY() - minY;
        double x2 = it->getTarget()->getX() - minX;
        double y2 = it->getTarget()->getY() - minY;
        bool dir = it->isDirected();
        bool loop = it->isLoop();

        if (loop) {
            double lx = x1 + VR * 0.7;
            double ly = y1 - VR * 1.2;
            double lr = VR * 0.8;
            ss << "<circle cx=\"" << lx << "\" cy=\"" << ly
               << "\" r=\"" << lr
               << "\" fill=\"none\" stroke=\"#505050\" stroke-width=\"2\"";
            if (dir) ss << " marker-end=\"url(#arrow)\"";
            ss << "/>\n";
        } else {
            double dx = x2 - x1, dy = y2 - y1;
            double dist = std::sqrt(dx * dx + dy * dy);
            if (dist > 0.001) {
                double nx = dx / dist, ny = dy / dist;
                double sx = x1 + nx * VR, sy = y1 + ny * VR;
                double ex = x2 - nx * VR, ey = y2 - ny * VR;
                ss << "<line x1=\"" << sx << "\" y1=\"" << sy
                   << "\" x2=\"" << ex << "\" y2=\"" << ey
                   << "\" stroke=\"#505050\" stroke-width=\"2\"";
                if (dir) ss << " marker-end=\"url(#arrow)\"";
                ss << "/>\n";
            }
        }

        if (it->getWeight() != 1.0) {
            std::string ws = std::to_string(it->getWeight());
            ws = ws.substr(0, ws.find('.') + 2);
            double mx, my;
            if (loop) { mx = x1 + VR * 0.7; my = y1 - VR * 2.5; }
            else { mx = (x1 + x2) / 2.0; my = (y1 + y2) / 2.0 - 10; }
            ss << "<text x=\"" << mx << "\" y=\"" << my
               << "\" text-anchor=\"middle\" font-family=\"Arial\" font-size=\"10\" "
               << "font-weight=\"bold\" fill=\"#a03232\">" << ws << "</text>\n";
        }
    }

    for (auto it = graph.verticesBegin(); it != graph.verticesEnd(); ++it) {
        double x = it->getX() - minX;
        double y = it->getY() - minY;
        ss << "<circle cx=\"" << x << "\" cy=\"" << y
           << "\" r=\"" << VR
           << "\" fill=\"#ffffff\" stroke=\"#323232\" stroke-width=\"2\"/>\n"
           << "<text x=\"" << x << "\" y=\"" << y
           << "\" text-anchor=\"middle\" dominant-baseline=\"central\" "
           << "font-family=\"Segoe UI, Arial, sans-serif\" font-size=\"16\" "
           << "font-weight=\"700\" fill=\"#000000\">"
           << it->getLabel() << "</text>\n";
    }

    ss << "</svg>\n";

    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + filename);
    file << ss.str();
    file.close();
}
