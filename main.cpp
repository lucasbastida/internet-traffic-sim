#include <iostream>
#include <fstream>
#include "DirectedEdge.h"
#include "EdgeWeightedDigraph.h"
#include "Dijkstra.h"
#include <stack>

using namespace std;

class Terminal
{
};

class Page
{
};

class Packet
{
};

int main()
{
    ifstream ifs("tinyEWD.txt");
    EdgeWeightedDigraph graph(ifs);
    ifs.close();
    cout << graph.toString() << endl;

    cout << "DijkstraSP:" << endl;
    Dijkstra sp(graph, 0);
    for (int t = 0; t < graph.getV(); t++)
    {
        if (sp.hasPathTo(t))
        {
            stack<DirectedEdge> s1 = sp.pathTo(t);
            cout << 0 << " to " << t << " ("<< sp.distTo(t) <<") : ";
            string path = "";
            while (!s1.empty())
            {
                path.append(s1.top().toString());
                path.append("  ");
                s1.pop();
            }
            cout << path << endl;
        }
        else
        {
            cout << 0 << "has no path to " << t << endl;
        }
    }
    return 0;
}