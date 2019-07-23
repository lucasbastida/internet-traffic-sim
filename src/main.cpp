#include <iostream>
#include <fstream>
#include "./graph/DirectedEdge.h"
#include "./graph/EdgeWeightedDigraph.h"
#include "./graph/Dijkstra.h"
#include <stack>

using namespace std;

int main()
{
    ifstream ifs("tinyEWD.txt");
    EdgeWeightedDigraph graph(ifs);
    ifs.close();
    cout << graph.toString() << endl;

    cout << "DijkstraSP:" << endl;
    int s = 3;
    Dijkstra sp(graph, s);
    for (int t = 0; t < graph.getV(); t++)
    {
        if (sp.hasPathTo(t))
        {
            stack<DirectedEdge> s1 = sp.pathTo(t);
            cout << s << " to " << t << " ("<< sp.distTo(t) <<") : ";
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