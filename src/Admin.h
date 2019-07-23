#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <fstream>

#include "Router.h"
#include "EdgeWeightedDigraph.h"
#include "DirectedEdge.h"
#include "Dijkstra.h"


class Admin
{
private:
    int V;
    int E;

    Router *routers;
    EdgeWeightedDigraph * G;

    void loadFile(std::string dir);
public:
    Admin(std::string dir);
    void calculateRouteTable(int v);

};

Admin::Admin(std::string dir)
{
    loadFile(dir);
}

void Admin::loadFile(std::string dir)
{
    std::ifstream ist("tinyEWD.txt");

	ist >> V >> E;

    if (V < 0)
        throw std::invalid_argument("|V| in a Digraph must be nonnegative");
    
    routers = new Router[V];
    G = new EdgeWeightedDigraph(V);

	for (int i = 0; i < E; i++)
	{
		int v, w;
		double weight;
		ist >> v;
		ist >> w;
		ist >> weight;
        DirectedEdge * edge = new DirectedEdge(v, w, weight);
		G->addEdge(edge);
	}

}

#endif