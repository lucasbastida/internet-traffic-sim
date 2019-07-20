#ifndef DIRECTED_EDGE_H
#define DIRECTED_EDGE_H

#include <exception>
#include <stdexcept>

class DirectedEdge
{
private:
    const int v;
    const int w;
    const double weight;

public:
    DirectedEdge(int v, int w, double weight);
    ~DirectedEdge();
    int from() { return v; }
    int to() { return w; }
    double getWeight() { return weight; }
    std::string toString(){ return (v + "->" + std::to_string(w) + " " + std::to_string(weight)); }
};

DirectedEdge::DirectedEdge(int v, int w, double weight) : v(v), w(w), weight(weight)
{
    if (v < 0 || w < 0)
    {
        throw std::invalid_argument("received negative value");
    }
}

DirectedEdge::~DirectedEdge()
{
}

#endif