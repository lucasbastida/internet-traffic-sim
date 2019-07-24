#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <fstream>

#include <random>

#include "./internet/Router.h"
#include "./graph/EdgeWeightedDigraph.h"
#include "./graph/DirectedEdge.h"
#include "./graph/Dijkstra.h"
#include "./internet/Page.h"

class Admin
{
private:
    int V;
    int E;
    int terminalAmount;

    std::default_random_engine generator;
    std::uniform_int_distribution<> dis;

    std::uniform_int_distribution<int> randomRouter;
    std::uniform_int_distribution<int> randomTerminal;
    std::uniform_int_distribution<int> probability;

    void loadFile(std::string dir);
    void turnOnRouters();
    void setRandomRouter(int a, int b) { randomRouter = std::uniform_int_distribution<int>(a, b); };
    void setRandomTerminal(int a, int b) { randomTerminal = std::uniform_int_distribution<int>(a, b); };
    void setProbability(int a, int b) { probability = std::uniform_int_distribution<int>(a, b); };

public:
    EdgeWeightedDigraph *G;
    Admin(std::string dir);
    void updateRouteTable();
    void cycle();
};

Admin::Admin(std::string dir)
{
    loadFile(dir);
    turnOnRouters();
    setRandomRouter(0, V - 1);
    setRandomTerminal(0, terminalAmount - 1);
    setProbability(0, 100);
}

void Admin::loadFile(std::string dir)
{
    std::ifstream ist("tinyEWD.txt");

    ist >> V;
    ist >> E;
    ist >> terminalAmount;
    if (V < 0)
        throw std::invalid_argument("|V| in a Digraph must be nonnegative");

    G = new EdgeWeightedDigraph(V);

    for (int i = 0; i < E; i++)
    {
        int v, w;
        double weight;
        ist >> v;
        ist >> w;
        ist >> weight;
        DirectedEdge *edge = new DirectedEdge(v, w, weight);
        G->addEdge(edge);
    }
    ist.close();
}

void Admin::updateRouteTable()
{

    for (int v = 0; v < V; v++)
    {
        G->nodes[v].updateEdgeWeights();
    }

    for (int i = 0; i < V; i++)
    {
        std::stack<DirectedEdge> *routeTable = new std::stack<DirectedEdge>[V];

        Dijkstra sp(*G, i);
        for (int t = 0; t < V; t++)
        {
            if (sp.hasPathTo(t))
            {
                routeTable[t] = sp.pathTo(t);
            }
        }
        for (int t = 0; t < G->getV(); t++)
        {
            if (sp.hasPathTo(t))
            {
                std::stack<DirectedEdge> s1 = sp.pathTo(t);
                std::cout << i << " to " << t << " (" << sp.distTo(t) << ") : ";
                std::string path = "";
                while (!s1.empty())
                {
                    path.append(s1.top().toString());
                    path.append("  ");
                    s1.pop();
                }
                std::cout << path << std::endl;
            }
            else
            {
                std::cout << 0 << "has no path to " << t << std::endl;
            }
        }
        G->nodes[i].updateRouteTable(routeTable);
    }
}

void Admin::turnOnRouters()
{

    for (int i = 0; i < V; i++)
    {
        G->nodes[i].initialize(terminalAmount, G->nodes[i].adj.front().from(), V, &generator);
    }
    updateRouteTable();
}

void Admin::cycle()
{
    //SEND PAGE if probability > 60.
    if (probability(generator) > 60)
    {

        int origin[2] = {randomRouter(generator), randomTerminal(generator)};
        int destination[2] = {randomRouter(generator), randomTerminal(generator)};

        if (origin[0] != destination[0])
        {
            Page *page = G->nodes[origin[0]].terminals[origin[1]].page;
            G->nodes[origin[0]].recievePage(page, destination);
            std::cout << "PROBABILITY OF SENDING PAGE OVER 60%...SENDING PAGE FROM-->TO:  " << origin[0] << "." << origin[1] << "-->" << destination[0] << "." << destination[1] << std::endl;
        }
    }

    //SEND ALL PACKETS IN EDGE QUEUE
    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < G->nodes[i].adj.size(); j++)
        {
            while (!G->nodes[i].edgeQueue[j].empty())
            {
                int destination = G->nodes[i].edgeQueue[j].front()->recieverIp[0];
                Packet *packet = G->nodes[i].edgeQueue[j].front();
                G->nodes[destination].buffer.push(packet);
                G->nodes[i].edgeQueue[j].pop();
            }
        }
    }

    //WORK ON PACKETS IN BUFFER. STORE PACKET TO GENERATE WEBPAGE OR REDIRECT TO EDGE QUEUE.
    for (int i = 0; i < V; i++)
    {
        G->nodes[i].recivePacket();
    }
}

#endif