#ifndef SVGRENDERER_H
#define SVGRENDERER_H

#include <string>

class Graph;

class SvgRenderer {
public:
    void render(const Graph& graph, const std::string& filename);
};

#endif // SVGRENDERER_H
