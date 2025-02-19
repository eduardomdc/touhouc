#include "packets.h"
#include <arpa/inet.h>
#include <stdio.h>
#include "server.h"
#include "../player.h"
#include "../compact_array.h"
#include "../bullets.h"
#include "client.h"
#include <string.h>

char packageBuffer[MAX_PACKAGE_BUFFER_SIZE] = {0};

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
    // unsigned int packageSize = 0;
    // UdpHeader udpHeader;
    // udpHeader.packetType = UDP_BULLET_ARRAY;
    // udpHeader.seq = 0;
    // memcpy(packageBuffer, &udpHeader, sizeof(udpHeader));
    // packageSize += sizeof(udpHeader);
    
    // UdpBulletArray udpBulletArray;
    // udpBulletArray.len = compactEnemyBulletArray.freeIndex;
    // udpBulletArray.team = team;
    // memcpy(packageBuffer+packageSize, &udpBulletArray, sizeof(udpBulletArray));
    // packageSize += sizeof(udpBulletArray);
    Bullet* bullets;
    UdpHeader udpHeader;
    udpHeader.packetType = UDP_BULLET_ARRAY;
    udpHeader.seq = 0;
    switch (team) {
        case ENEMY:
            bullets = enemyBullets;
            udpHeader.len = compactEnemyBulletArray.freeIndex;
            break;
        case PLAYER_1:
            bullets = playerBullets;
            udpHeader.len = compactPlayerBulletArray.freeIndex;
            break;
        default:
            return;
    }
    udpHeader.team = team;
    if (udpHeader.len == 0) return;
    socklen_t addrlen = sizeof(gameServer.clientAddress);
    if (sendto(gameServer.udpSock, &udpHeader, sizeof(udpHeader), 0, (struct sockaddr*)&gameServer.clientAddress, addrlen) < 0){
        fprintf(stderr, "Failed to send UDP header\n");
    }
    if (sendto(gameServer.udpSock, bullets, udpHeader.len*sizeof(Bullet), 0, (struct sockaddr*)&gameServer.clientAddress, addrlen) < 0){
        fprintf(stderr, "Failed to send UDP bullet data\n");
    };
}

void receiveUDPBulletArray(UdpHeader header) {
    Bullet* bullets;
    switch (header.team){
        case ENEMY:
            bullets = enemyBullets;
            compactEnemyBulletArray.freeIndex = header.len;
            break;
        case PLAYER_1:
            bullets = playerBullets;
            compactPlayerBulletArray.freeIndex = header.len;
            break;
        default:
            return;
    }
    socklen_t addrlen = sizeof(gameClient.serverAddress);
    recvfrom(gameClient.udpSock, bullets, sizeof(Bullet)*header.len, 0, (struct sockaddr*)&gameClient.serverAddress, &addrlen);
}

void sendUDPPlayerData(Team player){
    UdpHeader udpHeader;
    udpHeader.packetType = UDP_PLAYER_DATA;
    udpHeader.seq = 0;
}