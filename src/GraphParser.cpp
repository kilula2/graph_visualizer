#include "GraphParser.h"
#include <sstream>
#include <stdexcept>

Graph GraphParser::parse(const std::string& text) {
    Graph graph;

    std::istringstream stream(text);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(stream, line)) {
        if (!line.empty() && line.find_first_not_of(" \t\r") != std::string::npos)
            lines.push_back(line);
    }

    if (lines.empty()) throw std::runtime_error("Empty input");

    int vertexCount = parseVertexCount(lines[0]);

    if (lines.size() < 2) throw std::runtime_error("Missing vertex labels line");

    auto labels = parseLabels(lines[1]);

    if (static_cast<int>(labels.size()) != vertexCount)
        throw std::runtime_error(
            "Number of labels does not match vertex count: expected " +
            std::to_string(vertexCount) + ", got " + std::to_string(labels.size()));

    for (const auto& label : labels)
        graph.addVertex(label);

    for (size_t i = 2; i < lines.size(); ++i)
        parseEdge(lines[i], graph);

    return graph;
}

int GraphParser::parseVertexCount(const std::string& line) {
    try {
        int count = std::stoi(line);
        if (count <= 0)
            throw std::runtime_error("Vertex count must be positive: " + std::to_string(count));
        return count;
    } catch (const std::invalid_argument&) {
        throw std::runtime_error("Invalid vertex count: " + line);
    }
}

std::vector<std::string> GraphParser::parseLabels(const std::string& line) {
    std::vector<std::string> labels;
    std::istringstream iss(line);
    std::string label;
    while (iss >> label) labels.push_back(label);
    if (labels.empty()) throw std::runtime_error("Vertex labels line is empty");
    return labels;
}

void GraphParser::parseEdge(const std::string& line, Graph& graph) {
    std::istringstream iss(line);
    std::string a, op, b;
    double weight = 1.0;

    if (!(iss >> a))
        throw std::runtime_error("Invalid edge: " + line);

    if (!(iss >> op))
        throw std::runtime_error("Invalid edge: " + line);

    // Проверяем, является ли op оператором
    if (op == "-" || op == "->" || op == "<-" || op == "<->") {
        // a op b [weight]
        if (!(iss >> b))
            throw std::runtime_error("Invalid edge: " + line);

        std::string rest;
        if (iss >> rest) {
            try { weight = std::stod(rest); }
            catch (...) { throw std::runtime_error("Invalid weight: " + rest); }
        }

        if (op == "-") {
            graph.addEdge(a, b, false, weight);
        } else if (op == "->") {
            graph.addEdge(a, b, true, weight);
        } else if (op == "<-") {
            graph.addEdge(b, a, true, weight);
        } else if (op == "<->") {
            graph.addEdge(a, b, true, weight);
            graph.addEdge(b, a, true, weight);
        }
    } else {
        // Нет оператора: op — это вторая вершина
        b = op;

        // Проверяем вес
        std::string rest;
        if (iss >> rest) {
            try { weight = std::stod(rest); }
            catch (...) { throw std::runtime_error("Invalid weight: " + rest); }
        }

        graph.addEdge(a, b, false, weight);
    }
}
