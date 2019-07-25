#ifndef PACKET_H
#define PACKET_H

class Packet
{
public:
    int senderIp[2];
    int recieverIp[2];
    int packetId;
    int totalPackets;
    int pageSize;

    Packet(int senderIp[], int recieverIp[], int packetId, int totalPackets, int pageSize);
};

Packet::Packet(int senderIp[], int recieverIp[], int packetId, int totalPackets, int pageSize)
    : packetId(packetId), totalPackets(totalPackets), pageSize(pageSize)
{
    for (int i = 0; i < 2; i++)
    {
        this->senderIp[i] = senderIp[i];
    }

    for (int i = 0; i < 2; i++)
    {
        this->recieverIp[i] = recieverIp[i];
    }
}

#endif