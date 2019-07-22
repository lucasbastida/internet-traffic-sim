#ifndef DIRECTED_EDGE_H
#define DIRECTED_EDGE_H

#include <exception>
#include <stdexcept>

class DirectedEdge
{
private:
    int v;
    int w;
    double weight;

    double bandwidth;

public:
    DirectedEdge();
    DirectedEdge(int v, int w, double weight);
    ~DirectedEdge();
    int from() { return v; }
    int to() { return w; }
    double getWeight() { return weight; }
    std::string toString() { return (std::to_string(v) + "->" + std::to_string(w) + " " + std::to_string(weight)); }
    void updateWeight(int queue);
};

DirectedEdge::DirectedEdge() : v(0), w(0), weight(0.0)
{
}

DirectedEdge::DirectedEdge(int v, int w, double weight) : v(v), w(w), weight(weight), bandwidth(weight)
{
    if (v < 0 || w < 0)
    {
        throw std::invalid_argument("received negative value");
    }
}

void DirectedEdge::updateWeight(int queue)
{
    if (queue == 0)
    {
        weight = bandwidth;
    }
    
    else
    {
        weight = (queue/bandwidth);
    }
}

DirectedEdge::~DirectedEdge()
{
}

#endif