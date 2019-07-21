#include <iostream>
#include <fstream>
#include "DirectedEdge.h"
#include "EdgeWeightedDigraph.h"
#include "Dijkstra.h"
#include <stack>

using namespace std;

class Router
{
};

class Terminal
{
};

class Page
{
};

class Packet
{
};

class Admin
{
};

int main()
{
    ifstream ifs("mediumEWD.txt");
    EdgeWeightedDigraph graph(ifs);
    ifs.close();
    cout << graph.toString() << endl;

    cout << "DijkstraSP:" << endl;
    Dijkstra spDijkstra(graph, 0);
    cout << "Has path to X: " << spDijkstra.hasPathTo(6) << endl;
    if (spDijkstra.hasPathTo(6))
    {
        stack<DirectedEdge> s1 = spDijkstra.pathTo(6);
        while (!s1.empty())
        {
            cout<< s1.top().toString() << endl;
            s1.pop();
        }
    }

    return 0;
}