#include "packets.h"
#include <arpa/inet.h>
#include <stdio.h>
#include "server.h"
#include "../player.h"
#include "../compact_array.h"
#include "../bullets.h"
#include "client.h"
#include <string.h>

PacketBuffer packetBuffer = {0};

void sendTcpPlayerHit(short playerID);

void sendTcpPlayerData(short playerID, Player player){
    TcpHeader tcpHeader = {TCP_PLAYER_DATA};
    TcpPlayerData data = {0, player};
    send(gameServer.clientTCPSock, &tcpHeader, sizeof(tcpHeader), 0);
    send(gameServer.clientTCPSock, &data, sizeof(data), 0);
}

void receiveTcpPlayerData(TcpPlayerData tcpPlayerData){
    player = tcpPlayerData.player;
}

void sendTcpPlayerItemPickup(short playerID, ItemType itemType);

void sendUDPBulletArray(Team team) {
    resetPacketBuffer();
    UdpHeader udpHeader;
    udpHeader.packetType = UDP_BULLET_ARRAY;
    udpHeader.seq = 1;
    udpHeader.team = team;
    writePacketBuffer(&udpHeader, sizeof(UdpHeader));

    UdpBulletArray udpBulletArray;
    switch (team) {
        case ENEMY:
            udpBulletArray.len = compactEnemyBulletArray.freeIndex;
            writePacketBuffer(&udpBulletArray, sizeof(UdpBulletArray));
            writePacketBuffer(enemyBullets, sizeof(Bullet)*udpBulletArray.len);
            break;
        case PLAYER_1:
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

void receiveUDPBulletArray(UdpHeader header) {
    UdpBulletArray udpBulletArray;
    readPacketBuffer(&udpBulletArray, sizeof(udpBulletArray));
    switch (header.team){
    case ENEMY:
        readPacketBuffer(enemyBullets, sizeof(Bullet)*udpBulletArray.len);
        compactEnemyBulletArray.freeIndex = udpBulletArray.len;
        break;
    case PLAYER_1:
        readPacketBuffer(playerBullets, sizeof(Bullet)*udpBulletArray.len);
        compactPlayerBulletArray.freeIndex = udpBulletArray.len;
    default:
        break;
    }
}

void sendUDPPlayerData(Team teamPlayer){
    UdpHeader udpHeader;
    udpHeader.packetType = UDP_PLAYER_DATA;
    udpHeader.seq = 0;
    udpHeader.team = teamPlayer; // player 1 or player 2
    socklen_t addrlen = sizeof(gameServer.clientAddress);

    resetPacketBuffer();
    writePacketBuffer(&udpHeader, sizeof(UdpHeader));
    writePacketBuffer(&player, sizeof(Player));

    if (sendto(gameServer.udpSock, packetBuffer.bytes, packetBuffer.len, 0, (struct sockaddr*)&gameServer.clientAddress, addrlen) < 0){
        fprintf(stderr, "Failed to send UDP player data\n");
    };
}

void receiveUDPPlayerData(UdpHeader header){
    readPacketBuffer(&player, sizeof(Player));
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