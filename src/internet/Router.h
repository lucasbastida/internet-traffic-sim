//TODO ADD RANDOM PAGE SIZE
#ifndef ROUTER_H
#define ROUTER_H

#include <list>
#include <stack>
#include <queue>
#include "../graph/DirectedEdge.h"
#include "Packet.h"
#include "Terminal.h"

#define PACKET_SIZE 6

class Router
{
    int ip;
    int terminales;

    Terminal *terminals;

    std::stack<DirectedEdge> *routeTable;
    std::queue<Packet*> buffer;
    std::queue<Packet*> *edgeQueue;
    std::list<std::queue<Packet>> newPage;

    void redirectPacket(Packet* packet);
    void storePacket(Packet* packet);

public:
    std::list<DirectedEdge> adj;
public:
    void initialize(int numberTerminals, int V);
    void setIp(int ip) { this->ip = ip; };
    void updateRouteTable(std::stack<DirectedEdge> *routeTable);

    void recievePage(Page *page,int destination[]);
    void recivePacket();
};

void Router::initialize(int terminalAmount, int V)
{
    terminals = new Terminal[terminalAmount];
    routeTable = new std::stack<DirectedEdge>[V];
    edgeQueue = new std::queue<Packet*>[adj.size()];

    for (int i = 0; i < terminalAmount; i++)
    {
        terminals[i].page = new Page(i, 20); //TODO ADD RANDOM PAGE SIZE
    }
    
}

void Router::updateRouteTable(std::stack<DirectedEdge> *routeTable)
{
    delete this->routeTable;
    this->routeTable = routeTable;
}

void Router::recievePage(Page *page,int destination[])
{
    int senderIp[2] = {ip,page->id};

    double totalPackets = (page->size/PACKET_SIZE)+1;

    for (int i = 1; i < totalPackets; i++)
    {
        Packet * packet = new Packet(senderIp,destination,i,totalPackets,page->id,page->size);
        buffer.push(packet);
    }
    
}


void Router::recivePacket()
{
    for (int i = 0; i < buffer.size(); i++)
    {
        Packet* packet = buffer.front();

        if (packet->recieverIp[1] == this->ip)
        {
            storePacket(packet);
        }
        else
        {
            redirectPacket(packet);
        }

        buffer.pop();
    }
    
}

void Router::storePacket(Packet * packet)
{

}

void Router::redirectPacket(Packet * packet)
{
    
}
#endif