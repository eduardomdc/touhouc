#include "packets.h"
#include <arpa/inet.h>
#include <raylib.h>
#include <stdio.h>
#include "server.h"
#include "../player.h"
#include "../compact_array.h"
#include "../bullets.h"
#include "../spawner.h"
#include <string.h>

PacketBuffer packetBuffer = {0};

void sendTcpPlayerHit(PlayerCharacter character){
    TcpHeader tcpHeader = {TCP_PLAYER_HIT};
    TcpPlayerHit tcpPlayerHit = {character};
    resetPacketBuffer();
    writePacketBuffer(&tcpHeader, sizeof(TcpHeader));
    writePacketBuffer(&tcpPlayerHit, sizeof(TcpPlayerHit));
    send(gameServer.clientTCPSock, packetBuffer.bytes, packetBuffer.len, 0);
}

void receiveTcpPlayerHit(){
    // future get which player for hit effects
    PlaySound(assets.soundEffects[PLAYER_HIT]);
}

void sendTcpPlayerData(PlayerCharacter character, Player player){
    TcpHeader tcpHeader = {TCP_PLAYER_DATA};
    TcpPlayerData data = {character, player};
    resetPacketBuffer();
    writePacketBuffer(&tcpHeader, sizeof(tcpHeader));
    writePacketBuffer(&data, sizeof(data));
    send(gameServer.clientTCPSock, packetBuffer.bytes, packetBuffer.len, 0);
}

void receiveTcpPlayerData(){
    TcpPlayerData tcpPlayerData;
    readPacketBuffer(&tcpPlayerData, sizeof(tcpPlayerData));
    players[tcpPlayerData.character] = tcpPlayerData.player;
}

void sendTcpPlayerItemPickup(PlayerCharacter character, ItemType itemType){
    if (!gameServer.clientIsConnected) return;
    TcpHeader tcpHeader = {TCP_PLAYER_ITEM_PICK_UP};
    TcpPlayerItemPickUp tcpPlayerItemPickup;
    tcpPlayerItemPickup.character = character;
    tcpPlayerItemPickup.itemType = itemType;
    resetPacketBuffer();
    writePacketBuffer(&tcpHeader, sizeof(TcpHeader));
    writePacketBuffer(&tcpPlayerItemPickup, sizeof(TcpPlayerItemPickUp));
    send(gameServer.clientTCPSock, packetBuffer.bytes, packetBuffer.len, 0);
}

void receiveTcpPlayerItemPickup(){
    TcpPlayerItemPickUp tcpPlayerItemPickup;
    readPacketBuffer(&tcpPlayerItemPickup, sizeof(tcpPlayerItemPickup));
    ItemType type = tcpPlayerItemPickup.itemType;
    fprintf(stderr, "receive %d pickup\n", type);
    PlaySound(assets.soundEffects[itemData[type].pickupSound]);
}

void sendUDPBulletArray(Team team) {
    resetPacketBuffer();
    UdpHeader udpHeader;
    udpHeader.packetType = UDP_BULLET_ARRAY;
    writePacketBuffer(&udpHeader, sizeof(UdpHeader));

    UdpBulletArray udpBulletArray;
    udpBulletArray.team = team;
    switch (team) {
        case TEAM_ENEMY:
            udpBulletArray.len = compactEnemyBulletArray.freeIndex;
            writePacketBuffer(&udpBulletArray, sizeof(UdpBulletArray));
            writePacketBuffer(enemyBullets, sizeof(Bullet)*udpBulletArray.len);
            break;
        case TEAM_PLAYERS:
            udpBulletArray.len = compactPlayerBulletArray.freeIndex;
            writePacketBuffer(&udpBulletArray, sizeof(UdpBulletArray));
            writePacketBuffer(playerBullets, sizeof(Bullet)*udpBulletArray.len);
            break;
        default:
            return;
    }

    socklen_t addrlen = sizeof(gameServer.clientAddress);
    if (sendto(gameServer.udpSock, packetBuffer.bytes, packetBuffer.len, 0, (struct sockaddr*)&gameServer.clientAddress, addrlen) < 0){
        fprintf(stderr, "Failed to send UDP bullet array packet\n");
    }
}

void receiveUDPBulletArray() {
    UdpBulletArray udpBulletArray;
    readPacketBuffer(&udpBulletArray, sizeof(udpBulletArray));
    switch (udpBulletArray.team){
    case TEAM_ENEMY:
        readPacketBuffer(enemyBullets, sizeof(Bullet)*udpBulletArray.len);
        compactEnemyBulletArray.freeIndex = udpBulletArray.len;
        break;
    case TEAM_PLAYERS:
        readPacketBuffer(playerBullets, sizeof(Bullet)*udpBulletArray.len);
        compactPlayerBulletArray.freeIndex = udpBulletArray.len;
    default:
        break;
    }
}

void sendUDPPlayerData(PlayerCharacter character){
    UdpHeader udpHeader;
    udpHeader.packetType = UDP_PLAYER_DATA;
    resetPacketBuffer();
    writePacketBuffer(&udpHeader, sizeof(UdpHeader));
    
    UdpPlayerData udpPlayerData;
    udpPlayerData.character = character;
    writePacketBuffer(&udpPlayerData, sizeof(UdpPlayerData));
    
    writePacketBuffer(&players[character], sizeof(Player));

    socklen_t addrlen = sizeof(gameServer.clientAddress);
    if (sendto(gameServer.udpSock, packetBuffer.bytes, packetBuffer.len, 0, (struct sockaddr*)&gameServer.clientAddress, addrlen) < 0){
        fprintf(stderr, "Failed to send UDP player data\n");
    };
}

void sendUDPPlayerFire(){
    if (!gameServer.clientIsConnected) return;
    UdpHeader udpHeader;
    udpHeader.packetType = UDP_PLAYER_FIRE;
    resetPacketBuffer();
    writePacketBuffer(&udpHeader, sizeof(UdpHeader));
    socklen_t addrlen = sizeof(gameServer.clientAddress);
    if (sendto(gameServer.udpSock, packetBuffer.bytes, packetBuffer.len, 0, (struct sockaddr*)&gameServer.clientAddress, addrlen) < 0){
        fprintf(stderr, "Failed to send UDP player fire data\n");
    };
}

void receiveUDPPlayerFire(){
    PlaySound(assets.soundEffects[PLAYER_FIRE]);
}

void receiveUDPPlayerData(){
    UdpPlayerData udpPlayerData;
    readPacketBuffer(&udpPlayerData, sizeof(UdpPlayerData));
    readPacketBuffer(&players[udpPlayerData.character], sizeof(Player));
}

void resetPacketBuffer(){
    packetBuffer.len = 0;
}

void readPacketBuffer(void* dest, unsigned long size){
    memcpy(dest, packetBuffer.bytes+packetBuffer.len, size);
    packetBuffer.len += size;
}

void writePacketBuffer(void* src, unsigned long size){
    memcpy(packetBuffer.bytes+packetBuffer.len, src, size);
    packetBuffer.len += size;
}

void sendUDPItemData(){
    UdpHeader udpHeader = {UDP_ITEM_DATA};
    UdpItemData udpItemData = {itemsCArray.freeIndex};
    resetPacketBuffer();
    writePacketBuffer(&udpHeader, sizeof(udpHeader));
    writePacketBuffer(&udpItemData, sizeof(UdpItemData));
    writePacketBuffer(items, sizeof(Item)*udpItemData.len);
    socklen_t addrlen = sizeof(gameServer.clientAddress);
    if (sendto(gameServer.udpSock, packetBuffer.bytes, packetBuffer.len, 0, (struct sockaddr*)&gameServer.clientAddress, addrlen) < 0){
        fprintf(stderr, "Failed to send UDP player data\n");
    };
}

void receiveUDPItemData(){
    UdpItemData udpItemData;
    readPacketBuffer(&udpItemData, sizeof(UdpItemData));
    readPacketBuffer(items, udpItemData.len*sizeof(Item));
    itemsCArray.freeIndex = udpItemData.len;
}

void sendUDPEnemyData(){
    UdpHeader udpHeader = {UDP_ENEMY_DATA};
    resetPacketBuffer();
    writePacketBuffer(&udpHeader, sizeof(udpHeader));
    writePacketBuffer(enemyList, MAX_ENEMIES*sizeof(Enemy));
    socklen_t addrlen = sizeof(gameServer.clientAddress);
    if (sendto(gameServer.udpSock, packetBuffer.bytes, packetBuffer.len, 0, (struct sockaddr*)&gameServer.clientAddress, addrlen) < 0){
        fprintf(stderr, "Failed to send UDP player data\n");
    };
}

void receiveUDPEnemyData(){
    readPacketBuffer(enemyList, MAX_ENEMIES*sizeof(Enemy));
}

void sendTcpEnemyDeath(){
    TcpHeader header = {TCP_ENEMY_DEATH};
    resetPacketBuffer();
    writePacketBuffer(&header, sizeof(TcpHeader));
    send(gameServer.clientTCPSock, packetBuffer.bytes, packetBuffer.len, 0);
}

void receiveTcpEnemyDeath(){
    PlaySound(assets.soundEffects[ENEMY_HIT]);
}