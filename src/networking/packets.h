#ifndef PACKETS_H
#define PACKETS_H
#include "../items.h"

typedef enum {
    TCP_PLAYER_HIT,
    TCP_PLAYER_ONE_UP,
    TCP_PLAYER_ITEM_PICK_UP,
    TCP_PACKET_TYPES_LEN,
} TCPPacketTypes;

typedef struct TcpPlayerHit {
    short playerID;
} TcpPlayerHit;

typedef struct TcpPlayerOneUp {
    short playerID;
} TcpPlayerOneUp;

typedef struct TcpPlayerItemPickUp {
    short playerID;
    ItemType itemType;
} TcpPlayerOneUp;

#endif