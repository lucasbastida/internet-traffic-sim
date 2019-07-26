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
    std::cout << "-------------------------------SETTING GRAPH-------------------------------" << std::endl;
    ist >> V;
    ist >> E;
    ist >> terminalAmount;

    if (V < 1 || V > 256)
        throw std::invalid_argument("Invalid |V| value: " + std::to_string(V) + " Must be within [1,256]");
    if (terminalAmount < 1 || terminalAmount > 256)
        throw std::invalid_argument("Invalid terminalAmount value: " + std::to_string(terminalAmount) + " Must be within [1,256]");
        
    G = new EdgeWeightedDigraph(V);

    for (int i = 0; i < E; i++)
    {
        int v, w;
        int weight;
        ist >> v;
        ist >> w;
        ist >> weight;
        if (v < 0 || v > 255)
            throw std::invalid_argument("Invalid Edge v value: " + std::to_string(v));
        if (w < 0 || w > 255)
            throw std::invalid_argument("Invalid Edge w value: " + std::to_string(v));
        DirectedEdge edge(v, w, weight, &(G->nodes[v]), &(G->nodes[w]));
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
            else
            {
                routeTable[t] = std::stack<DirectedEdge>();
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
                std::cout << i << "has no path to " << t << std::endl;
            }
        }
        G->nodes[i].updateRouteTable(routeTable);
    }
    std::cout << "-------------------------FINISHED UPDATING ROUTERS ROUTE TABLE----------------------" << std::endl;
}

void Admin::turnOnRouters()
{
    std::cout << "-------------------------------TURNING ON ROUTERS-------------------------------" << std::endl;
    for (int i = 0; i < V; i++)
    {
        G->nodes[i].initialize(terminalAmount, G->nodes[i].adj.front().from(), V, &generator);
    }
    updateRouteTable();
    std::cout << "---------------------------FINISHED TURNING ON ROUTERS-------------------------------" << std::endl;
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
            std::cout << "PROBABILITY OF SENDING PAGE OVER " << 100 - pageCreationPercentage
                      << "%...SENDING PAGE (FROM->TO):  "
                      << origin[0] << "." << origin[1] << "->" << destination[0] << "." << destination[1] << std::endl;
            G->nodes[origin[0]].receivePage(origin[1], destination);
        }
    }

    //SEND ALL PACKETS IN EDGE QUEUE
    for (int i = 0; i < V; i++)
    {
        G->nodes[i].sendPackets();
    }

    //WORK ON PACKETS IN BUFFER. STORE PACKET TO GENERATE WEBPAGE OR REDIRECT TO EDGE QUEUE.
    for (int i = 0; i < V; i++)
    {
        G->nodes[i].receivePackets();
    }
}

Admin::~Admin()
{
    delete G;
}

#endif