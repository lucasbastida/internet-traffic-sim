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
    int pageCreationPercentage = 50;

    std::random_device mch;
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
    void setGenerator(std::random_device &mch) { generator = std::default_random_engine(mch()); }

public:
    int terminalAmount;
    EdgeWeightedDigraph *G;
    Admin(std::string dir);
    ~Admin();
    void updateRouteTable();
    void cycle();
};

Admin::Admin(std::string dir)
{
    loadFile(dir);
    setGenerator(mch);
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

    std::cout << "-------------------------------SETTING GRAPH-------------------------------" << std::endl;
    G = new EdgeWeightedDigraph(V);

    for (int i = 0; i < E; i++)
    {
        int v, w;
        double weight;
        ist >> v;
        ist >> w;
        ist >> weight;
        DirectedEdge edge(v, w, weight);
        G->addEdge(edge);
    }
    std::cout << "GRAPH:" << std::endl;
    std::cout << G->toString() << std::endl;
    std::cout << "-------------------------------FINISHED SETTING GRAPH-------------------------------" << std::endl;
    ist.close();
}

void Admin::updateRouteTable()
{
    std::cout << "-------------------------------UPDATING ROUTERS ROUTE TABLE-------------------------------" << std::endl;
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
    std::cout << "-------------------------------SETTING ROUTERS-------------------------------" << std::endl;
    for (int i = 0; i < V; i++)
    {
        G->nodes[i].initialize(terminalAmount, G->nodes[i].adj.front().from(), V, &generator);
    }
    updateRouteTable();
}

void Admin::cycle()
{

    //SEND PAGE if probability > .
    if (probability(generator) > 100 - pageCreationPercentage)
    {

        int origin[2] = {randomRouter(generator), randomTerminal(generator)};
        int destination[2] = {randomRouter(generator), randomTerminal(generator)};

        if (origin[0] != destination[0])
        {
            std::cout << "PROBABILITY OF SENDING PAGE OVER " << 100 - pageCreationPercentage << "%...SENDING PAGE (FROM->TO):  " << origin[0] << "." << origin[1] << "->" << destination[0] << "." << destination[1] << std::endl;
            
            G->nodes[origin[0]].recievePage(origin[1], destination);
        }
    }

    //SEND ALL PACKETS IN EDGE QUEUE
    for (int i = 0; i < V; i++)
    {
        int bufferIndex = 0;
        for (std::list<DirectedEdge>::iterator it = G->nodes[i].adj.begin(); it != G->nodes[i].adj.end(); ++it)
        {
            int destination = it->to();
            for (int j = 0; j < it->maxPackets; j++)
            {
                if (!G->nodes[i].edgeQueue[bufferIndex].empty())
                {
                    Packet packet = G->nodes[i].edgeQueue[bufferIndex].front();
                    G->nodes[destination].buffer.push(packet);
                    G->nodes[i].edgeQueue[bufferIndex].pop();
                }
            }
            bufferIndex++;
        }
    }

    //WORK ON PACKETS IN BUFFER. STORE PACKET TO GENERATE WEBPAGE OR REDIRECT TO EDGE QUEUE.
    for (int i = 0; i < V; i++)
    {
        G->nodes[i].recivePacket();
    }
}

Admin::~Admin()
{
    delete G;
}

#endif