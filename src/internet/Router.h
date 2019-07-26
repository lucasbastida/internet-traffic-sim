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
    void receivePage(int terminal, int destination[]);
    void receivePackets();
    void sendPackets();
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
    delete[] tempRouteTable;

    for (std::list<DirectedEdge>::iterator it = adj.begin(); it != adj.end(); ++it)
    {
        std::stack<DirectedEdge> temp;
        temp.push(*it);
        routeTable[it->to()] = temp;
    }
}

void Router::receivePage(int terminal, int destination[])
{
    terminals[terminal].pagesSent++;
    Page *page = terminals[terminal].page;
    int senderIp[2] = {ip, terminal};

    int totalPackets = (page->size / PACKET_SIZE) + 1;

    std::cout << "------------------------------------------------------------------------------" << std::endl;
    std::cout << "Recieved page at ip: (" << this->ip << "." << terminals[terminal].id << ") page size:" << page->size << std::endl;
    std::cout << "Dividing into packets:" << std::endl;
    std::cout << "packet amount:" << totalPackets << std::endl;
    std::cout << "Adding packets into router buffer. current size:" << buffer.size() << std::endl;

    for (int i = 0; i < totalPackets; i++)
    {
        Packet packet(senderIp, destination, i, totalPackets, page->size);
        buffer.push(packet);
    }
    std::cout << "Finished adding packets into buffer " << ip << " . new buffer size:" << buffer.size() << std::endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;
}

void Router::receivePackets()
{
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    std::cout << "Packet reception from router: " << this->ip << std::endl;
    std::cout << "router buffer size " << buffer.size() << std::endl;
    while (!buffer.empty())
    {
        Packet packet = buffer.front();

        if (packet.recieverIp[0] == this->ip)
        {
            storePacket(packet);
        }
        else
        {
            redirectPacket(packet);
        }
        buffer.pop();
        std::cout << "new router buffer size " << buffer.size() << std::endl;
    }
    std::cout << "Finished packet reception from router: " << this->ip << std::endl;
    std::cout << "final router buffer size " << buffer.size() << std::endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;
}

void Router::storePacket(Packet packet)
{
    std::cout << "STORING packet: (" << ip << "->" << packet.recieverIp[0] << ") (ip->dest.ip)" << std::endl;
    bool stored = false;
    packetsStored++;
    for (std::list<std::queue<Packet>>::iterator it = newPage.begin(); it != newPage.end(); ++it)
    {
        if (!it->empty())
        {
            if (it->front().recieverIp[1] == packet.recieverIp[1])
            {
                std::cout << "There is a page already being creating for corresponding packet. Adding packet." << std::endl;
                it->push(packet);
                stored = true;
            }
        }
    }

    if (!stored)
    {
        std::cout << "No page currently being created for corresponding packet" << std::endl;
        std::cout << "Creating new page originating from (" << packet.senderIp[0] << "." << packet.senderIp[1] << ") at router ip:"
                  << this->ip << std::endl;
        std::queue<Packet> temp;
        temp.push(packet);
        newPage.push_back(temp);
    }

    for (std::list<std::queue<Packet>>::iterator it = newPage.begin(); it != newPage.end(); ++it)
    {
        int queueSize = it->size();
        if (queueSize == it->front().totalPackets)
        {
            std::cout << "Page from: (" << packet.senderIp[0] << "." << packet.senderIp[1]
                      << ") to: (" << packet.recieverIp[0] << "." << packet.recieverIp[1]
                      << ") of size: " << it->front().pageSize << " was created!" << std::endl;
            // std::cout << it->front().totalPackets*PACKET_SIZE <<std::endl;
            terminals[it->front().recieverIp[1]].pagesRecieved++;
            while (!it->empty())
            {
                it->pop();
            }
            newPage.erase(it++);
        }
    }
}

void Router::redirectPacket(Packet packet)
{
    std::cout << "REDIRECTING packet to edge queue (" << ip << "->" << packet.recieverIp[0] << ")" << std::endl;
    int index = 0;
    if (adj.begin() == adj.end())
    {
        std::cout << "NO EDGES CONNECTING ROUTER, deleting packet id:" << packet.packetId << " from:" << packet.senderIp[0] << "." << packet.senderIp[1] << std::endl;
    }
    for (std::list<DirectedEdge>::iterator it = adj.begin(); it != adj.end(); ++it)
    {
        //if the connected outgoing edge is the one corresponding to the path the packet needs to take, then add it to that edges queue
        if (!routeTable[packet.recieverIp[0]].empty())
        {
            if (routeTable[packet.recieverIp[0]].top().to() == it->to())
            {
                edgeQueue[index].push(packet);
                std::cout << "EdgeQueue ("
                          << it->from() << "->" << it->to()
                          << "). EdgeQueue size: " << edgeQueue[index].size() << std::endl;
            }
        }
        else
        {
            std::cout << "UNREACHABLE DESTINATION FROM " << packet.senderIp[0] << "." << packet.senderIp[1] << " deleting packet..." << std::endl;
        }
        index++;
    }
}

void Router::sendPackets()
{
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    int bufferIndex = 0;
    std::cout << "Sending packets from Edge Queues at router ip:" << this->ip << std::endl;
    for (std::list<DirectedEdge>::iterator it = adj.begin(); it != adj.end(); ++it)
    {
        // int destination = it->to();
        std::cout << "Edge Queue (" << it->from() << "->" << it->to() << ") size:" << edgeQueue[bufferIndex].size()
                  << " - Edge Bandwidth:" << it->bandwidth
                  << std::endl;
        for (int j = 0; j < it->maxPackets; j++)
        {
            if (!edgeQueue[bufferIndex].empty())
            {
                Packet packet = edgeQueue[bufferIndex].front();
                std::cout << "sending packet #:" << j << " - packet id:" << packet.packetId << std::endl;
                it->dest->buffer.push(packet);
                edgeQueue[bufferIndex].pop();
            }
        }
        bufferIndex++;
    }
    std::cout << "Finished sending packets from Edge Queues at router ip:" << this->ip << std::endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;
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

Router::~Router()
{
    delete[] routeTable;
    delete[] edgeQueue;
    delete[] terminals;
}
#endif