#ifndef PACKETS_H
#define PACKETS_H
#define MAX_PACKAGE_BUFFER_SIZE 32000
#include "../items.h"
#include "../player.h"
#include "../bullets.h"

typedef enum {
    TCP_PLAYER_HIT,
    TCP_PLAYER_DATA,
    TCP_PLAYER_ITEM_PICK_UP,
    TCP_PACKET_TYPE_LEN,
} TcpPacketType;

typedef struct TcpHeader {
    TcpPacketType packetType;
} __attribute__((packed)) TcpHeader;

typedef struct TcpPlayerHit {
    short playerID;
} __attribute__((packed)) TcpPlayerHit;

typedef struct TcpPlayerData {
    short playerID;
    Player player;
} __attribute__((packed)) TcpPlayerData;

typedef struct TcpPlayerItemPickUp {
    short playerID;
    ItemType itemType;
} __attribute__((packed)) TcpPlayerItemPickUp;

void sendTcpPlayerHit(short playerID);
void sendTcpPlayerData(short playerID, Player player);
void receiveTcpPlayerData(TcpPlayerData tcpPlayerData);
void sendTcpPlayerItemPickup(short playerID, ItemType itemType);

typedef enum {
    UDP_BULLET_ARRAY,
    UDP_PLAYER_DATA,
    UDP_PACKET_TYPE_LEN
} UdpPacketType;

typedef struct UdpHeader {
    UdpPacketType packetType;
    unsigned int seq;
    unsigned int len;
    Team team;
} __attribute__((packed)) UdpHeader;

typedef struct UdpBulletArray {
    Team team;
    int len;
} __attribute__((packed)) UdpBulletArray;

void sendUDPBulletArray(Team team);
void sendUDPPlayerData(Team teamPlayer);
void receiveUDPPlayerData(UdpHeader header);
void receiveUDPBulletArray(UdpHeader header);

extern char packageBuffer[MAX_PACKAGE_BUFFER_SIZE];

#endif