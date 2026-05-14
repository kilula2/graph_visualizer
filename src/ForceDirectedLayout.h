#ifndef FORCEDIRECTEDLAYOUT_H
#define FORCEDIRECTEDLAYOUT_H

class Graph;

class ForceDirectedLayout {
public:
    static ForceDirectedLayout& getInstance();

    void setParameters(double repulsion, double attraction,
                       int iterations, double edgeLength);
    void resetToDefaults();
    void run(Graph& graph);

    ForceDirectedLayout(const ForceDirectedLayout&) = delete;
    ForceDirectedLayout& operator=(const ForceDirectedLayout&) = delete;

private:
    ForceDirectedLayout();

    void initializePositions(Graph& graph);
    void calculateRepulsion(Graph& graph);
    void calculateAttraction(Graph& graph);
    void calculateEdgeRepulsion(Graph& graph);
    void calculateCenterGravity(Graph& graph);
    void updatePositions(Graph& graph);

    double repulsionForce_;
    double attractionForce_;
    int iterations_;
    double idealEdgeLength_;
    double temperature_;
    double edgeRepulsionForce_;
    double centerGravityForce_;
    double width_;
    double height_;
};

#endif // FORCEDIRECTEDLAYOUT_H
