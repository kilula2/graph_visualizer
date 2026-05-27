#ifndef GRAPHPARSER_H
#define GRAPHPARSER_H

#include <string>
#include <vector>
#include "Graph.h"

class GraphParser {
public:
    Graph parse(const std::string& text);

private:
    int parseVertexCount(const std::string& line);
    std::vector<std::string> parseLabels(const std::string& line);
    void parseEdge(const std::string& line, Graph& graph);
};

#endif // GRAPHPARSER_H
