#ifndef ROUTER_H
#define ROUTER_H

#include <list>
#include <stack>
#include "DirectedEdge.h"

class Router
{
    int ip;
    int terminales;

    std::stack<DirectedEdge> *routeTable;

public:
    std::list<DirectedEdge> adj;

    void setIp(int ip) { this->ip = ip; };
    void setRouteTable();
};

void Router::setRouteTable()
{
    routeTable = new std::stack<DirectedEdge>[adj.size()];
}



#endif