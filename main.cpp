#include <iostream>
#include <fstream>
#include "DirectedEdge.h"
#include "EdgeWeightedDigraph.h"

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
    cout<< graph.toString() <<endl;

    return 0;
}