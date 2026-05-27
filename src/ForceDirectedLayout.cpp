#include "ForceDirectedLayout.h"
#include "Graph.h"
#include "Vertex.h"
#include "Edge.h"
#include <cmath>
#include <algorithm>
#include <queue>
#include <vector>

ForceDirectedLayout& ForceDirectedLayout::getInstance() {
    static ForceDirectedLayout instance;
    return instance;
}

ForceDirectedLayout::ForceDirectedLayout()
    : repulsionForce_(5000.0)
    , attractionForce_(0.005)
    , iterations_(300)
    , idealEdgeLength_(200.0)
    , temperature_(1.0)
    , edgeRepulsionForce_(3000.0)
    , centerGravityForce_(0.005)
    , width_(800.0)
    , height_(600.0)
{
}

void ForceDirectedLayout::setParameters(double r, double a, int i, double e) {
    repulsionForce_ = r;
    attractionForce_ = a;
    iterations_ = i;
    idealEdgeLength_ = e;
}

void ForceDirectedLayout::resetToDefaults() {
    repulsionForce_ = 5000.0;
    attractionForce_ = 0.005;
    iterations_ = 300;
    idealEdgeLength_ = 200.0;
    temperature_ = 1.0;
    edgeRepulsionForce_ = 3000.0;
    centerGravityForce_ = 0.005;
}

// ----------------------------------------------------------------
// ПОЛУЧЕНИЕ СОСЕДЕЙ ПО ИНДЕКСУ
// ----------------------------------------------------------------
static std::vector<int> getNeighbors(const Graph& graph, int idx) {
    std::vector<int> result;
    int i = 0;
    for (auto vit = graph.verticesBegin(); vit != graph.verticesEnd(); ++vit, ++i) {
        if (i == idx) {
            for (auto eit = graph.edgesBegin(); eit != graph.edgesEnd(); ++eit) {
                if (eit->getSource() == &(*vit)) {
                    int j = 0;
                    for (auto v2 = graph.verticesBegin(); v2 != graph.verticesEnd(); ++v2, ++j) {
                        if (eit->getTarget() == &(*v2)) {
                            result.push_back(j);
                            break;
                        }
                    }
                }
                if (eit->getTarget() == &(*vit)) {
                    int j = 0;
                    for (auto v2 = graph.verticesBegin(); v2 != graph.verticesEnd(); ++v2, ++j) {
                        if (eit->getSource() == &(*v2)) {
                            result.push_back(j);
                            break;
                        }
                    }
                }
            }
            break;
        }
    }
    return result;
}

// ----------------------------------------------------------------
// РАСКЛАДКА ПО КРУГУ (циклы, полные графы, звёзды)
// ----------------------------------------------------------------
static void layoutCircle(Graph& graph, double w, double h, int centerIdx = -1) {
    int V = graph.getVertexCount();
    double cx = w / 2.0;
    double cy = h / 2.0;

    if (centerIdx >= 0) {
        // Звезда: центр фиксирован, остальные по кругу
        graph.getVertices()[centerIdx].setX(cx);
        graph.getVertices()[centerIdx].setY(cy);
        double radius = std::min(w, h) * 0.38;
        int idx = 0;
        for (int i = 0; i < V; ++i) {
            if (i == centerIdx) continue;
            double angle = 2.0 * M_PI * idx / (V - 1) - M_PI / 2.0;
            graph.getVertices()[i].setX(cx + radius * cos(angle));
            graph.getVertices()[i].setY(cy + radius * sin(angle));
            ++idx;
        }
    } else {
        // Обычный круг
        double radius = std::min(w, h) * 0.38;
        for (int i = 0; i < V; ++i) {
            double angle = 2.0 * M_PI * i / V - M_PI / 2.0;
            graph.getVertices()[i].setX(cx + radius * cos(angle));
            graph.getVertices()[i].setY(cy + radius * sin(angle));
        }
    }
}

// ----------------------------------------------------------------
// РАСКЛАДКА ДЕРЕВА (корень сверху)
// ----------------------------------------------------------------
static void layoutTree(Graph& graph, double w, double h) {
    int V = graph.getVertexCount();
    if (V == 0) return;

    // Степени
    std::vector<int> degree(V, 0);
    for (auto eit = graph.edgesBegin(); eit != graph.edgesEnd(); ++eit) {
        int si = 0, ti = 0, idx = 0;
        for (auto vit = graph.verticesBegin(); vit != graph.verticesEnd(); ++vit, ++idx) {
            if (eit->getSource() == &(*vit)) si = idx;
            if (eit->getTarget() == &(*vit)) ti = idx;
        }
        degree[si]++;
        degree[ti]++;
    }

    // Корень — вершина с макс. степенью
    int root = 0;
    for (int i = 0; i < V; ++i)
        if (degree[i] > degree[root]) root = i;

    // BFS
    std::vector<int> parent(V, -1);
    std::vector<int> depth(V, 0);
    std::vector<std::vector<int>> levels(V);
    std::queue<int> q;
    q.push(root);
    parent[root] = -1;
    depth[root] = 0;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        levels[depth[u]].push_back(u);
        for (int v : getNeighbors(graph, u)) {
            if (v != parent[u] && parent[v] == -1) {
                parent[v] = u;
                depth[v] = depth[u] + 1;
                q.push(v);
            }
        }
    }

    int maxDepth = 0;
    for (int i = 0; i < V; ++i)
        if (!levels[i].empty()) maxDepth = i;

    double margin = 60.0;
    double availW = w - 2 * margin;
    double availH = h - 2 * margin;

    for (int d = 0; d <= maxDepth; ++d) {
        int n = levels[d].size();
        double y = margin + (maxDepth > 0 ? d * availH / maxDepth : availH / 2.0);
        double spacing = (n > 1) ? availW / (n - 1) : 0;
        double startX = (n > 1) ? margin : w / 2.0;

        for (int i = 0; i < n; ++i) {
            double x = (n == 1) ? w / 2.0 : startX + i * spacing;
            graph.getVertices()[levels[d][i]].setX(x);
            graph.getVertices()[levels[d][i]].setY(y);
        }
    }
}

// ----------------------------------------------------------------
// СИЛЫ ДЛЯ ОБЫЧНЫХ ГРАФОВ
// ----------------------------------------------------------------
void ForceDirectedLayout::calculateRepulsion(Graph& graph) {
    auto& verts = graph.getVertices();
    int n = verts.size();
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double dx = verts[i].getX() - verts[j].getX();
            double dy = verts[i].getY() - verts[j].getY();
            double dist = sqrt(dx * dx + dy * dy);
            if (dist < 1.0) dist = 1.0;
            double f = repulsionForce_ / (dist * dist);
            verts[i].setDx(verts[i].getDx() + dx / dist * f);
            verts[i].setDy(verts[i].getDy() + dy / dist * f);
            verts[j].setDx(verts[j].getDx() - dx / dist * f);
            verts[j].setDy(verts[j].getDy() - dy / dist * f);
        }
    }
}

void ForceDirectedLayout::calculateAttraction(Graph& graph) {
    for (const auto& edge : graph.getEdges()) {
        if (edge.isLoop()) continue;

        Vertex* v1 = edge.getSource();
        Vertex* v2 = edge.getTarget();

        double dx = v1->getX() - v2->getX();
        double dy = v1->getY() - v2->getY();
        double distance = std::sqrt(dx * dx + dy * dy);

        if (distance < 1.0) distance = 1.0;

        double force = attractionForce_ * edge.getWeight() * (distance - idealEdgeLength_);

        double fx = (dx / distance) * force;
        double fy = (dy / distance) * force;

        v1->setDx(v1->getDx() - fx);
        v1->setDy(v1->getDy() - fy);
        v2->setDx(v2->getDx() + fx);
        v2->setDy(v2->getDy() + fy);
    }
}

void ForceDirectedLayout::calculateEdgeRepulsion(Graph& graph) {
    auto& verts = graph.getVertices();
    for (auto& v : verts) {
        for (auto& e : graph.getEdges()) {
            if (&v == e.getSource() || &v == e.getTarget()) continue;
            double ax = e.getSource()->getX(), ay = e.getSource()->getY();
            double bx = e.getTarget()->getX(), by = e.getTarget()->getY();
            double dx = bx - ax, dy = by - ay;
            double len2 = dx * dx + dy * dy;
            if (len2 < 0.01) continue;
            double t = ((v.getX() - ax) * dx + (v.getY() - ay) * dy) / len2;
            t = std::max(0.0, std::min(1.0, t));
            double cx = ax + t * dx, cy = ay + t * dy;
            double ddx = v.getX() - cx, ddy = v.getY() - cy;
            double dist = sqrt(ddx * ddx + ddy * ddy);
            if (dist < 45.0 && dist > 0.01) {
                double f = edgeRepulsionForce_ / (dist * dist);
                v.setDx(v.getDx() + ddx / dist * f);
                v.setDy(v.getDy() + ddy / dist * f);
            }
        }
    }
}

void ForceDirectedLayout::calculateCenterGravity(Graph& graph) {
    double cx = width_ / 2.0, cy = height_ / 2.0;
    for (auto& v : graph.getVertices()) {
        v.setDx(v.getDx() + (cx - v.getX()) * centerGravityForce_);
        v.setDy(v.getDy() + (cy - v.getY()) * centerGravityForce_);
    }
}

void ForceDirectedLayout::updatePositions(Graph& graph) {
    for (auto& v : graph.getVertices()) {
        double dx = v.getDx(), dy = v.getDy();
        double mag = sqrt(dx * dx + dy * dy);
        if (mag > 0.0) {
            double lim = std::min(mag, temperature_);
            dx = dx / mag * lim;
            dy = dy / mag * lim;
        }
        double nx = v.getX() + dx;
        double ny = v.getY() + dy;
        nx = std::max(40.0, std::min(width_ - 40.0, nx));
        ny = std::max(40.0, std::min(height_ - 40.0, ny));
        v.setX(nx);
        v.setY(ny);
    }
}

// ----------------------------------------------------------------
// ГЛАВНЫЙ RUN
// ----------------------------------------------------------------
void ForceDirectedLayout::run(Graph& graph) {
    int V = graph.getVertexCount();
    int E = graph.getEdgeCount();
    if (V == 0) return;

    // 1. Цикл
    if (E == V && V >= 3) {
        bool ok = true;
        for (int i = 0; i < V; ++i)
            if ((int)getNeighbors(graph, i).size() != 2) { ok = false; break; }
        if (ok) { layoutCircle(graph, width_, height_); return; }
    }

    // 2. Звезда
    for (int i = 0; i < V; ++i) {
        if ((int)getNeighbors(graph, i).size() == V - 1) {
            layoutCircle(graph, width_, height_, i);
            return;
        }
    }

    // 3. Дерево
    if (E == V - 1) {
        layoutTree(graph, width_, height_);
        return;
    }

    // 4. Полный граф
    if (E == V * (V - 1) / 2) {
        layoutCircle(graph, width_, height_);
        return;
    }

    // 5. Обычный граф — force-directed с гарантированным расстоянием
    layoutCircle(graph, width_, height_); // старт с круга

    temperature_ = std::min(width_, height_) / 3.0;
    for (int iter = 0; iter < iterations_; ++iter) {
        for (auto& v : graph.getVertices()) v.resetDisplacement();
        calculateRepulsion(graph);
        calculateAttraction(graph);
        calculateEdgeRepulsion(graph);
        calculateCenterGravity(graph);
        updatePositions(graph);
        temperature_ *= 0.97;
        if (temperature_ < 0.1) temperature_ = 0.1;
    }

    // Финальное принудительное расталкивание: минимум 150 пикселей
    for (int pass = 0; pass < 20; ++pass) {
        bool moved = false;
        for (int i = 0; i < V; ++i) {
            for (int j = i + 1; j < V; ++j) {
                Vertex& a = graph.getVertices()[i];
                Vertex& b = graph.getVertices()[j];
                double dx = a.getX() - b.getX();
                double dy = a.getY() - b.getY();
                double dist = sqrt(dx * dx + dy * dy);
                if (dist < 150.0 && dist > 0.01) {
                    double push = (150.0 - dist) / 2.0;
                    double nx = dx / dist, ny = dy / dist;
                    a.setX(a.getX() + nx * push);
                    a.setY(a.getY() + ny * push);
                    b.setX(b.getX() - nx * push);
                    b.setY(b.getY() - ny * push);
                    moved = true;
                }
            }
        }
        if (!moved) break;
    }
}
