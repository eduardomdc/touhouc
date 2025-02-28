#include "packets.h"
#include <arpa/inet.h>
#include <raylib.h>
#include <stdio.h>
#include "server.h"
#include "client.h"
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

    socklen_t addrlen = sizeof(gameServer.udpClientAddress);
    if (sendto(gameServer.udpSock, packetBuffer.bytes, packetBuffer.len, 0, (struct sockaddr*)&gameServer.udpClientAddress, addrlen) < 0){
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

void sendUDPSfx(short sfx){
    if (!gameServer.clientIsConnected) return;
    UdpHeader udpHeader;
    udpHeader.packetType = UDP_SFX;
    UdpSfx udpSfx;
    udpSfx.sfx = sfx;
    resetPacketBuffer();
    writePacketBuffer(&udpHeader, sizeof(UdpHeader));
    writePacketBuffer(&udpSfx, sizeof(UdpSfx));
    socklen_t addrlen = sizeof(gameServer.udpClientAddress);
    if (sendto(gameServer.udpSock, packetBuffer.bytes, packetBuffer.len, 0, (struct sockaddr*)&gameServer.udpClientAddress, addrlen) < 0){
        fprintf(stderr, "Failed to send UDP player fire data\n");
    };
}

void receiveUDPSfx(){
    UdpSfx udpSfx;
    readPacketBuffer(&udpSfx, sizeof(UdpSfx));
    PlaySound(assets.soundEffects[udpSfx.sfx]);
}

void sendUDPPlayerData(PlayerCharacter character){
    UdpHeader udpHeader;
    udpHeader.packetType = UDP_PLAYER_DATA;
    resetPacketBuffer();
    writePacketBuffer(&udpHeader, sizeof(UdpHeader));
    
    UdpPlayerData udpPlayerData;
    udpPlayerData.character = character;
    udpPlayerData.pos = players[character].pos;
    udpPlayerData.alive = players[character].alive;
    udpPlayerData.score = players[character].score;
    udpPlayerData.lives = players[character].lives;
    writePacketBuffer(&udpPlayerData, sizeof(UdpPlayerData));
    
    socklen_t addrlen = sizeof(gameServer.udpClientAddress);
    if (sendto(gameServer.udpSock, packetBuffer.bytes, packetBuffer.len, 0, (struct sockaddr*)&gameServer.udpClientAddress, addrlen) < 0){
        fprintf(stderr, "Failed to send UDP player data\n");
    };
}

void receiveUDPPlayerData(){
    UdpPlayerData udpPlayerData;
    readPacketBuffer(&udpPlayerData, sizeof(UdpPlayerData));
    players[udpPlayerData.character].pos = udpPlayerData.pos;
    players[udpPlayerData.character].score = udpPlayerData.score;
    players[udpPlayerData.character].alive = udpPlayerData.alive;
    players[udpPlayerData.character].lives = udpPlayerData.lives;
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
    socklen_t addrlen = sizeof(gameServer.udpClientAddress);
    if (sendto(gameServer.udpSock, packetBuffer.bytes, packetBuffer.len, 0, (struct sockaddr*)&gameServer.udpClientAddress, addrlen) < 0){
        fprintf(stderr, "Failed to send UDP item data\n");
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
    UdpEnemyArray udpEnemyArray = {compactEnemyArray.freeIndex};
    resetPacketBuffer();
    writePacketBuffer(&udpHeader, sizeof(udpHeader));
    writePacketBuffer(&udpEnemyArray, sizeof(udpEnemyArray));
    writePacketBuffer(enemyList, compactEnemyArray.freeIndex*sizeof(Enemy));
    socklen_t addrlen = sizeof(gameServer.udpClientAddress);
    if (sendto(gameServer.udpSock, packetBuffer.bytes, packetBuffer.len, 0, (struct sockaddr*)&gameServer.udpClientAddress, addrlen) < 0){
        fprintf(stderr, "Failed to send UDP enemy data\n");
    }
}

void receiveUDPEnemyData(){
    UdpEnemyArray udpEnemyArray;
    readPacketBuffer(&udpEnemyArray, sizeof(UdpEnemyArray));
    readPacketBuffer(enemyList, udpEnemyArray.len*sizeof(Enemy));
    compactEnemyArray.freeIndex = udpEnemyArray.len;
}

void sendTcpEnemyDeath(){
    if (!gameServer.clientIsConnected) return;
    TcpHeader header = {TCP_ENEMY_DEATH};
    resetPacketBuffer();
    writePacketBuffer(&header, sizeof(TcpHeader));
    send(gameServer.clientTCPSock, packetBuffer.bytes, packetBuffer.len, 0);
}

void receiveTcpEnemyDeath(){
    PlaySound(assets.soundEffects[SFX_ENEMY_DEATH]);
}

void sendUDPInputData(Input input, PlayerCharacter character){
    UdpHeader udpHeader = {UDP_INPUT_DATA};
    UdpInputData udpInputData = {character, input};
    resetPacketBuffer();
    writePacketBuffer(&udpHeader, sizeof(UdpHeader));
    writePacketBuffer(&udpInputData, sizeof(UdpInputData));
    socklen_t addrlen = sizeof(gameClient.udpServerAddress);
    //char ipAddr[INET_ADDRSTRLEN];
    //inet_ntop(AF_INET, &gameClient.udpServerAddress.sin_addr, ipAddr, INET_ADDRSTRLEN);
    //fprintf(stderr, "Sending upd input data to %s\n", ipAddr, (int) ntohs(gameClient.udpServerAddress.sin_port));
    int bytesSent = sendto(gameClient.udpSock, packetBuffer.bytes, packetBuffer.len, 0, (struct sockaddr*)&gameClient.udpServerAddress, addrlen);
    if (bytesSent < 0){
        fprintf(stderr, "Failed to send input data to server\n");
    } else if (bytesSent != sizeof(UdpHeader) + sizeof(UdpInputData)){
        fprintf(stderr, "Sent malformed input data to server %d\n", bytesSent);
    }
}
void receiveUDPInputData(){
    UdpInputData udpInputData;
    readPacketBuffer(&udpInputData, sizeof(UdpInputData));
    updatePlayer(&players[udpInputData.character], udpInputData.input);
}
