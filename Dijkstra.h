#ifndef DIJKSTRA_H
#define DIJKSTRA_h
#include "EdgeWeightedDigraph.h"
#include "DirectedEdge.h"
#include "BinaryHeapIndexMinPQ.h"
#include <limits>
#include <stack>
#include <assert.h>

class Dijkstra
{
private:
    BinaryHeapIndexMinPQ<double> *pq;
    double *arrayDistTo;
    DirectedEdge *edgeTo;

    void relax(EdgeWeightedDigraph &G, int v);
    
public:
    Dijkstra(EdgeWeightedDigraph &G, int s);
    ~Dijkstra();

    double distTo(int v);
    double hasPathTo(int v);
    std::stack<DirectedEdge> pathTo(int v);
};

Dijkstra::Dijkstra(EdgeWeightedDigraph &G, int s)
{
    arrayDistTo = new double[G.getV()];
    edgeTo = new DirectedEdge[G.getV()];
    pq = new BinaryHeapIndexMinPQ<double>(G.getV());

    for (int i = 0; i < G.getV(); i++)
    {
        arrayDistTo[i] = std::numeric_limits<double>::infinity();
    }

    arrayDistTo[s] = 0.0;
    edgeTo[s] = DirectedEdge(s, s, 0.0);
    pq->Insert(s, 0.0);
    while (!pq->IsEmpty())
    {
        int v = pq->DelMin();
        relax(G, v);
    }
}

void Dijkstra::relax(EdgeWeightedDigraph &G, int v)
{
    std::list<DirectedEdge> b = G.adjList(v);
    for (std::list<DirectedEdge>::iterator it = b.begin(); it != b.end(); ++it)
    {
        DirectedEdge e = *it;
        int w = e.to();
        if (arrayDistTo[w] > arrayDistTo[v] + e.getWeight())
        {
            arrayDistTo[w] = arrayDistTo[v] + e.getWeight();
            edgeTo[w] = e;
            if (pq->Contain(w))
            {
                pq->Change(w, arrayDistTo[w]);
            }
            else
            {
                pq->Insert(w, arrayDistTo[w]);
            }
        }
    }
}

double Dijkstra::distTo(int v)
{
    return arrayDistTo[v];
}

double Dijkstra::hasPathTo(int v)
{
    return arrayDistTo[v] != std::numeric_limits<double>::infinity();
}

std::stack<DirectedEdge> Dijkstra::pathTo(int v)
{
    assert(hasPathTo(v));
    std::stack<DirectedEdge> s;
    while (edgeTo[v].from() != v)
    {
        s.push(edgeTo[v]);
        v = edgeTo[v].from();
    }
    return s;
}


Dijkstra::~Dijkstra()
{
    if (pq)
    {
        delete pq;
    }
    if (edgeTo)
    {
        delete[] edgeTo;
    }
    if (arrayDistTo)
    {
        delete[] arrayDistTo;
    }
}

#endif