#ifndef EDGE_WEIGHTED_DIGRAPH_H
#define EDGE_WEIGHTED_DIGRAPH_H

#include <iostream>
#include <list>
#include <stdexcept>
#include <istream>
#include "DirectedEdge.h"
#include "../internet/Router.h"

class EdgeWeightedDigraph
{
private:
    int V;
    int E;

    void validateVertex(int v);

public:
    Router *nodes;

    EdgeWeightedDigraph(std::istream &ist);
    EdgeWeightedDigraph(int V);
    ~EdgeWeightedDigraph();
    int getE() { return E; }
    int getV() { return V; }
    void addEdge(DirectedEdge e);
    std::list<DirectedEdge> adjList(int v);
    std::string toString();
};

EdgeWeightedDigraph::EdgeWeightedDigraph(int V) : V(V)
{
    if (V < 0)
    {
        throw std::invalid_argument("|V| in a Digraph must be nonnegative");
    }

    E = 0;

    nodes = new Router[V];
    std::cout << "loaded " << V << " routers in graph" << std::endl;
}

EdgeWeightedDigraph::EdgeWeightedDigraph(std::istream &ist) : V(0), E(0)
{
    ist >> V >> E;
    if (V < 0)
    {
        throw std::invalid_argument("|V| in a Digraph must be nonnegative");
    }

    nodes = new Router[V];
    std::cout << "loaded " << V << " routers in graph" << std::endl;

    for (int i = 0; i < E; i++)
    {
        int v, w;
        double weight;
        ist >> v;
        ist >> w;
        ist >> weight;
        DirectedEdge edge(v,w,weight, &nodes[v], &nodes[w]);
        addEdge(edge);
        E--;
    }
}

void EdgeWeightedDigraph::validateVertex(int v)
{
    if (v < 0 || v >= V)
        throw std::invalid_argument("vertex " + std::to_string(v) + " is not between 0 and " + std::to_string((V - 1)));
}

void EdgeWeightedDigraph::addEdge(DirectedEdge e)
{
    int v = e.from();
    int w = e.to();
    validateVertex(v);
    validateVertex(w);

    nodes[v].adj.push_back(e);
    // std::cout << "ADDED EDGE: " << e->toString() << std::endl;
    E++;
}

std::list<DirectedEdge> EdgeWeightedDigraph::adjList(int v)
{
    validateVertex(v);

    return nodes[v].adj;
}

std::string EdgeWeightedDigraph::toString()
{
    std::string s = "";
    s.append(std::to_string(V) + " routers " + std::to_string(E) + " edges\n");
    for (int v = 0; v < V; v++)
    {
        s.append("router ip:" + std::to_string(v) + " : ");
        for (std::list<DirectedEdge>::iterator it = nodes[v].adj.begin(); it != nodes[v].adj.end(); ++it)
        {
            s.append(it->toString() + " ");
        }
        s.append("\n");
    }
    return s;
}

EdgeWeightedDigraph::~EdgeWeightedDigraph()
{
    if (nodes)
    {
        delete[] nodes;
    }
}
#endif