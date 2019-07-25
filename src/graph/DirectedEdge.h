#ifndef DIRECTED_EDGE_H
#define DIRECTED_EDGE_H

#include <exception>
#include <stdexcept>

class DirectedEdge
{
private:
    int v;
    int w;
    int weight;

public:
    int maxPackets;
    int bandwidth;
    DirectedEdge();
    DirectedEdge(int v, int w, int bandwidth);
    ~DirectedEdge();
    int from() { return v; }
    int to() { return w; }
    double getWeight() { return weight; }
    std::string toString() { return (std::to_string(v) + "->" + std::to_string(w) + " " + std::to_string(weight) + "~bw:" + std::to_string(bandwidth)); }
    void updateWeight(int queueSize, int packetSize);
};

DirectedEdge::DirectedEdge() : v(0), w(0), weight(1)
{
}

DirectedEdge::DirectedEdge(int v, int w, int bandwidth) : v(v), w(w), weight(1), bandwidth(bandwidth)
{
    if (v < 0 || w < 0 || bandwidth < 0)
    {
        throw std::invalid_argument("received negative value");
    }
}

void DirectedEdge::updateWeight(int queueSize, int packetSize)
{
    weight = 1 + (queueSize * packetSize / bandwidth);
    maxPackets = (bandwidth / packetSize);
}

DirectedEdge::~DirectedEdge()
{
}

#endif