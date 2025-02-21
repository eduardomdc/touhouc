#ifndef PACKETS_H
#define PACKETS_H
#define MAX_PACKAGE_BUFFER_SIZE 32000
#include "../items.h"
#include "../player.h"
#include "../bullets.h"
#include "../input.h"

typedef enum {
    TCP_PLAYER_HIT,
    TCP_PLAYER_DATA,
    TCP_PLAYER_ITEM_PICK_UP,
    TCP_ENEMY_DEATH,
    TCP_PACKET_TYPE_LEN,
} TcpPacketType;

typedef struct TcpHeader {
    TcpPacketType packetType;
} __attribute__((packed)) TcpHeader;

typedef struct TcpPlayerHit {
    PlayerCharacter character;
} __attribute__((packed)) TcpPlayerHit;

typedef struct TcpPlayerData {
    PlayerCharacter character;
    Player player;
} __attribute__((packed)) TcpPlayerData;

typedef struct TcpPlayerItemPickUp {
    PlayerCharacter character;
    ItemType itemType;
} __attribute__((packed)) TcpPlayerItemPickUp;

void sendTcpPlayerHit(PlayerCharacter character);
void receiveTcpPlayerHit();
void sendTcpPlayerData(PlayerCharacter character, Player player);
void receiveTcpPlayerData();
void sendTcpPlayerItemPickup(PlayerCharacter character, ItemType itemType);
void receiveTcpPlayerItemPickup();
void sendTcpEnemyDeath();
void receiveTcpEnemyDeath();

typedef enum {
    UDP_BULLET_ARRAY,
    UDP_PLAYER_DATA,
    UDP_PLAYER_FIRE,
    UDP_ENEMY_DATA,
    UDP_ITEM_DATA,
    UDP_INPUT_DATA,
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
    PlayerCharacter character;
} __attribute__((packed)) UdpPlayerData;

typedef struct UdpItemData {
    int len;
} __attribute__((packed)) UdpItemData;

typedef struct UdpInputData {
    PlayerCharacter character;
    Input input;
} __attribute__((packed)) UdpInputData;

void sendUDPBulletArray(Team team);
void sendUDPPlayerData(PlayerCharacter character);
void receiveUDPPlayerData();
void receiveUDPBulletArray();
void sendUDPPlayerFire();
void receiveUDPPlayerFire();
void sendUDPEnemyData();
void receiveUDPEnemyData();
void sendUDPItemData();
void receiveUDPItemData();
void sendUDPInputData(Input input, PlayerCharacter character);
void receiveUDPInputData();

typedef struct PacketBuffer {
    char bytes[MAX_PACKAGE_BUFFER_SIZE];
    unsigned int len;
} PacketBuffer;

extern PacketBuffer packetBuffer;

void resetPacketBuffer();
void readPacketBuffer(void* dest, unsigned long size);
void writePacketBuffer(void* src, unsigned long size);

#endif
