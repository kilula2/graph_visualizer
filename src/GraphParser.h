#ifndef GRAPHPARSER_H
#define GRAPHPARSER_H

#include <QString>
#include "Graph.h"

class GraphParser {
public:
    Graph parse(const QString& text);

private:
    int parseVertexCount(const QString& line);
    QStringList parseLabels(const QString& line);
    void parseEdge(const QString& line, Graph& graph);
};

#endif // GRAPHPARSER_H
