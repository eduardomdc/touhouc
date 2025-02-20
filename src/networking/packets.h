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
    Team tplayer;
} __attribute__((packed)) TcpPlayerHit;

typedef struct TcpPlayerData {
    Team tplayer;
    Player player;
} __attribute__((packed)) TcpPlayerData;

typedef struct TcpPlayerItemPickUp {
    Team tplayer;
    ItemType itemType;
} __attribute__((packed)) TcpPlayerItemPickUp;

void sendTcpPlayerHit(Team tplayer);
void receiveTcpPlayerHit(TcpPlayerHit tcpPlayerHit);
void sendTcpPlayerData(Team tplayer, Player player);
void receiveTcpPlayerData(TcpPlayerData tcpPlayerData);
void sendTcpPlayerItemPickup(Team tplayer, ItemType itemType);
void receiveTcpPlayerItemPickup(TcpPlayerItemPickUp tcpPlayerItemPickup);

typedef enum {
    UDP_BULLET_ARRAY,
    UDP_PLAYER_DATA,
    UDP_PLAYER_FIRE,
    UDP_ENEMY_DATA,
    UDP_ITEM_DATA,
    UDP_PACKET_TYPE_LEN
} UdpPacketType;

typedef struct UdpHeader {
    UdpPacketType packetType;
} __attribute__((packed)) UdpHeader;

typedef struct UdpBulletArray {
    int len;
    Team team;
} __attribute__((packed)) UdpBulletArray;

typedef struct UdpPlayerData {
    Team player;
} __attribute__((packed)) UdpPlayerData;

typedef struct UdpItemData {
    int len;
} __attribute__((packed)) UdpItemData;

void sendUDPBulletArray(Team team);
void sendUDPPlayerData(Team tplayer);
void receiveUDPPlayerData();
void receiveUDPBulletArray();
void sendUDPPlayerFire();
void receiveUDPPlayerFire();
void sendUDPEnemyData();
void receiveUDPEnemyData();
void sendUDPItemData();
void receiveUDPItemData();

typedef struct PacketBuffer {
    char bytes[MAX_PACKAGE_BUFFER_SIZE];
    unsigned int len;
} PacketBuffer;

extern PacketBuffer packetBuffer;

void resetPacketBuffer();
void readPacketBuffer(void* dest, unsigned long size);
void writePacketBuffer(void* src, unsigned long size);

#endif
