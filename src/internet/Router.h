//TODO ADD RANDOM PAGE SIZE
#ifndef ROUTER_H
#define ROUTER_H

#include <list>
#include <stack>
#include <queue>
#include <random>

#include "../graph/DirectedEdge.h"
#include "Packet.h"
#include "Terminal.h"

#define PACKET_SIZE 5

class Router
{
    int ip;
    int numberTerminals;
    int packetsStored = 0;

    std::stack<DirectedEdge> *routeTable;

    std::list<std::queue<Packet>> newPage;

    void redirectPacket(Packet packet);
    void storePacket(Packet packet);

public:
    ~Router();
    std::list<DirectedEdge> adj;
    std::queue<Packet> *edgeQueue;
    std::queue<Packet> buffer;

    Terminal *terminals;

public:
    void initialize(int numberTerminals, int ip, int V, std::default_random_engine *generator);
    void updateRouteTable(std::stack<DirectedEdge> *routeTable);

    void updateEdgeWeights();
    void recievePage(int terminal, int destination[]);
    void recivePacket();
    std::string toString();
};

void Router::initialize(int terminalAmount, int ip, int V, std::default_random_engine *generator)
{
    std::cout << "INITIALIZING ROUTER: IP: " << ip << std::endl;

    this->ip = ip;
    this->numberTerminals = terminalAmount;

    terminals = new Terminal[terminalAmount];
    routeTable = new std::stack<DirectedEdge>[V];
    edgeQueue = new std::queue<Packet>[adj.size()];

    std::uniform_int_distribution<int> distribution(10, 100);
    for (int i = 0; i < terminalAmount; i++)
    {
        terminals[i].id = i;
        terminals[i].page = new Page(distribution(*generator));
        //std::cout << "CREATED TERMINAL: " << terminals[i].toString() << std::endl;
        //std::cout << "CREATED PAGE FOR TERMINAL: " << terminals[i].page->toString() << std::endl;
    }
}

void Router::updateRouteTable(std::stack<DirectedEdge> *routeTable)
{
    std::stack<DirectedEdge> *tempRouteTable = this->routeTable;
    this->routeTable = routeTable;
    delete [] tempRouteTable;

    for (std::list<DirectedEdge>::iterator it = adj.begin(); it != adj.end(); ++it)
    {
        std::stack<DirectedEdge> temp;
        temp.push(*it);
        routeTable[it->to()] = temp;
    }
}

void Router::recievePage(int terminal, int destination[])
{
    terminals[terminal].pagesSent++;
    Page *page = terminals[terminal].page;
    int senderIp[2] = {ip, terminal};
    

    std::cout << "DEVIDING INTO PACKETS: page size:" << page->size << std::endl;
    double totalPackets = (page->size / PACKET_SIZE) + 1;
    std::cout << "packet amount->" << totalPackets << std::endl;

    for (int i = 0; i < totalPackets; i++)
    {
        Packet packet(senderIp, destination, i, totalPackets, page->size);
        // Packet *packet = new Packet(senderIp, destination, i, totalPackets, page->size);
        buffer.push(packet);
    }
    std::cout << "router " << ip << " buffer size " << buffer.size() << std::endl;
}

void Router::recivePacket()
{
    while (!buffer.empty())
    {
        Packet packet = buffer.front();

        if (packet.recieverIp[0] == this->ip)
        {
            std::cout << "STORING packet (ip:" << ip << "->dest.ip:" << packet.recieverIp[0] << ")" << std::endl;
            storePacket(packet);
            std::cout << "router buffer size " << buffer.size() << std::endl;
        }
        else
        {
            std::cout << "REDIRECTING packet to edge queue (" << ip << "->" << packet.recieverIp[0] << ")" << std::endl;
            redirectPacket(packet);
            std::cout << "router buffer size " << buffer.size() << std::endl;
        }

        buffer.pop();
        std::cout << "router buffer size " << buffer.size() << std::endl;
    }
}

void Router::storePacket(Packet packet)
{
    bool stored = false;
    packetsStored++;
    for (std::list<std::queue<Packet>>::iterator it = newPage.begin(); it != newPage.end(); ++it)
    {
        if (!it->empty())
        {
            if (it->front().recieverIp[1] == packet.recieverIp[1])
            {
                it->push(packet);
                stored = true;
            }
        }
    }

    if (!stored)
    {
        std::queue<Packet> temp;
        temp.push(packet);
        newPage.push_back(temp);
    }

    for (std::list<std::queue<Packet>>::iterator it = newPage.begin(); it != newPage.end(); ++it)
    {
        int queueSize = it->size();
        if (queueSize == it->front().totalPackets)
        {
            terminals[it->front().recieverIp[1]].pagesRecieved++;
            while (!it->empty())
            {
                // Packet packetQueue = it->front();
                it->pop();
                // delete packetQueue;
            }
            newPage.erase(it++);
        }
    }
}

void Router::redirectPacket(Packet packet)
{
    int index = 0;
    for (std::list<DirectedEdge>::iterator it = adj.begin(); it != adj.end(); ++it)
    {
        //if the connected outgoing edge is the one corresponding to the path the packet needs to take, then add it to that edges queue
        if (routeTable[packet.recieverIp[0]].top().to() == it->to())
        {
            edgeQueue[index].push(packet);
            std::cout << it->from() << "->" << it->to() << " new queue size: " << edgeQueue[index].size() << std::endl;
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
    return info;
}

Router::~Router(){
    delete [] routeTable;
    delete[] edgeQueue;
    delete[] terminals;
    
}
#endif