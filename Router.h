#ifndef ROUTER_H
#define ROUTER_H

#include <list>
#include "DirectedEdge.h"

class Router
{
    int ip;
    public:

    std::list<DirectedEdge> adj;

    void setIp(int ip) { this->ip=ip; };
};



#endif