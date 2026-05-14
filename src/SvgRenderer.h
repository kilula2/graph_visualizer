#ifndef SVGRENDERER_H
#define SVGRENDERER_H

#include <QString>

class Graph;

class SvgRenderer {
public:
    void render(const Graph& graph, const QString& filename);
};

#endif // SVGRENDERER_H
