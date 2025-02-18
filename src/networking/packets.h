#ifndef PACKETS_H
#define PACKETS_H
#include "../items.h"
#include "../player.h"

typedef enum {
    TCP_PLAYER_HIT,
    TCP_PLAYER_DATA,
    TCP_PLAYER_ITEM_PICK_UP,
    TCP_PACKET_TYPES_LEN,
} TCPPacketTypes;

typedef struct TcpHeader {
    TCPPacketTypes packetType;
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

#endif