#include "GraphParser.h"
#include <QStringList>
#include <stdexcept>

Graph GraphParser::parse(const QString& text) {
    Graph graph;

    QStringList lines = text.split('\n', Qt::SkipEmptyParts);

    if (lines.isEmpty()) {
        throw std::runtime_error("Пустой ввод");
    }

    int vertexCount = parseVertexCount(lines[0]);

    if (lines.size() < 2) {
        throw std::runtime_error("Отсутствует строка с метками вершин");
    }

    QStringList labels = parseLabels(lines[1]);

    if (labels.size() != vertexCount) {
        throw std::runtime_error(
            "Количество меток не совпадает с числом вершин: ожидалось " +
            std::to_string(vertexCount) + ", получено " +
            std::to_string(labels.size()));
    }

    for (const auto& label : labels) {
        graph.addVertex(label);
    }

    for (int i = 2; i < lines.size(); ++i) {
        parseEdge(lines[i], graph);
    }

    return graph;
}

int GraphParser::parseVertexCount(const QString& line) {
    bool ok = false;
    int count = line.trimmed().toInt(&ok);

    if (!ok) {
        throw std::runtime_error(
            "Некорректное количество вершин: " + line.toStdString());
    }

    if (count <= 0) {
        throw std::runtime_error(
            "Количество вершин должно быть положительным: " +
            std::to_string(count));
    }

    return count;
}

QStringList GraphParser::parseLabels(const QString& line) {
    QStringList labels = line.trimmed().split(' ', Qt::SkipEmptyParts);

    if (labels.isEmpty()) {
        throw std::runtime_error("Строка с метками вершин пуста");
    }

    return labels;
}

void GraphParser::parseEdge(const QString& line, Graph& graph) {
    QStringList parts = line.trimmed().split(' ', Qt::SkipEmptyParts);

    if (parts.size() != 2) {
        throw std::runtime_error(
            "Некорректный формат ребра: " + line.toStdString());
    }

    graph.addEdge(parts[0], parts[1]);
}
