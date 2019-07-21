//NOTE: *indegree not pointing to object

#ifndef EDGE_WEIGHTED_DIGRAPH_H
#define EDGE_WEIGHTED_DIGRAPH_H

#include <iostream>
#include <list>
#include <stdexcept>
#include <istream>
#include "DirectedEdge.h"

class EdgeWeightedDigraph
{
private:
    int V;
    int E;
    std::list<DirectedEdge> *adj;
    int *indegree;

    void validateVertex(int v);
public:
    EdgeWeightedDigraph(std::istream& ist);
    EdgeWeightedDigraph(int V);
    ~EdgeWeightedDigraph();
    int getE() { return E; }
    int getV() { return V; }
    void addEdge(DirectedEdge *e);
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
    indegree = new int[V];
    adj = new std::list<DirectedEdge>[V];
}

EdgeWeightedDigraph::EdgeWeightedDigraph(std::istream& ist) : V(0),E(0)
{
	ist >> V >> E;
    if (V < 0)
    {
        throw std::invalid_argument("|V| in a Digraph must be nonnegative");
    }
    adj = new std::list<DirectedEdge>[V];
	for (int i = 0; i < E; i++)
	{
		int v, w;
		double dWeight;
		ist >> v;
		ist >> w;
		ist >> dWeight;
        DirectedEdge * edge = new DirectedEdge(v, w, dWeight);
		this->addEdge(edge);
		E--;
	}
}

EdgeWeightedDigraph::~EdgeWeightedDigraph()
{
}

void EdgeWeightedDigraph::validateVertex(int v)
{
    if (v < 0 || v >= V)
        throw std::invalid_argument("vertex " + std::to_string(v) + " is not between 0 and " + std::to_string((V - 1)));
}

void EdgeWeightedDigraph::addEdge(DirectedEdge* e)
{
    int v = e->from();
    int w = e->to();
    validateVertex(v);
    validateVertex(w);
    adj[v].push_back(*e);
    //indegree[w]++;
    E++;
}

std::list<DirectedEdge> EdgeWeightedDigraph::adjList(int v)
{
    validateVertex(v);
    return adj[v];
}

std::string EdgeWeightedDigraph::toString()
{
    std::string s = "";
    s.append(std::to_string(V) + " " + std::to_string(E) + "\n");
    for (int v = 0; v < V; v++)
    {
        s.append(std::to_string(v) + ": ");
        for (std::list<DirectedEdge>::iterator it = adj[v].begin(); it != adj[v].end(); ++it)
        {
            s.append(it->toString() + " ");
        }
        s.append("\n");
    }
    return s;
}

#endif