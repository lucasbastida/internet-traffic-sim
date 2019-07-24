//TODO ADD RANDOM PAGE SIZE
#ifndef ROUTER_H
#define ROUTER_H

#include <list>
#include <stack>
#include <queue>

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>  

#include "../graph/DirectedEdge.h"
#include "Packet.h"
#include "Terminal.h"

#define PACKET_SIZE 6

class Router
{
    int ip;
    int numberTerminals;
    int packetsStored = 0;

    std::stack<DirectedEdge> *routeTable;

    std::list<std::queue<Packet>> newPage;

    void redirectPacket(Packet *packet);
    void storePacket(Packet *packet);

public:
    std::list<DirectedEdge> adj;
    std::queue<Packet *> *edgeQueue;
    std::queue<Packet *> buffer;

    Terminal *terminals;

public:
    void initialize(int numberTerminals, int ip, int V);
    void updateRouteTable(std::stack<DirectedEdge> *routeTable);

    void updateEdgeWeights();
    void recievePage(Page *page, int destination[]);
    void recivePacket();
    std::string toString();
};

void Router::initialize(int terminalAmount, int ip, int V)
{
    this->ip = ip;
    this->numberTerminals = terminalAmount;
    srand (time(NULL));

    terminals = new Terminal[terminalAmount];
    routeTable = new std::stack<DirectedEdge>[V];
    edgeQueue = new std::queue<Packet *>[adj.size()];

    for (int i = 0; i < terminalAmount; i++)
    {
        terminals[i].page = new Page(i, 20); //TODO ADD RANDOM PAGE SIZE
    }
}

void Router::updateRouteTable(std::stack<DirectedEdge> *routeTable)
{
    this->routeTable = routeTable;

    for (std::list<DirectedEdge>::iterator it = adj.begin(); it != adj.end(); ++it)
    {
        std::stack<DirectedEdge> temp;
        temp.push(*it);
        routeTable[it->to()] = temp;
    }
}

void Router::recievePage(Page *page, int destination[])
{
    int senderIp[2] = {ip, page->id};

    double totalPackets = (page->size / PACKET_SIZE) + 1;

    for (int i = 1; i < totalPackets; i++)
    {
        Packet *packet = new Packet(senderIp, destination, i, totalPackets, page->id, page->size);
        buffer.push(packet);
    }
}

void Router::recivePacket()
{
    for (int i = 0; i < buffer.size(); i++)
    {
        Packet *packet = buffer.front();

        if (packet->recieverIp[0] == this->ip)
        {
            std::cout <<"-------------------------------"<< packet->recieverIp[0]<<this->ip<<std::endl;
            storePacket(packet);
        }
        else
        {
            redirectPacket(packet);
        }

        buffer.pop();
    }
}

void Router::storePacket(Packet *packet)
{
    delete packet;
    packetsStored++;
}

void Router::redirectPacket(Packet *packet)
{
    int index = 0;
    for (std::list<DirectedEdge>::iterator it = adj.begin(); it != adj.end(); ++it)
    {
        if (routeTable[packet->recieverIp[0]].top().to() == it->to())
        {
            std::cout<< routeTable[packet->recieverIp[0]].top().to()<<"~~~~~~~~" <<it->to() <<std::endl;
            edgeQueue[index].push(packet);
            std::cout<<it->to()<<"size: "<<edgeQueue[index].size()<<std::endl;
        }
        index++;
    }
}

void Router::updateEdgeWeights()
{
    int index = 0;
    for (std::list<DirectedEdge>::iterator it = adj.begin(); it != adj.end(); ++it)
    {
        it->updateWeight(edgeQueue[index].size(), PACKET_SIZE);
        index++;
    }
}

std::string Router::toString()
{
    std::string info = "";
    info.append("router ip:" + std::to_string(ip) + " - packets stored:" + std::to_string(packetsStored) + " - amout of terminals:" + std::to_string(numberTerminals));
    info.append("\n");
    return info;
}

#endif