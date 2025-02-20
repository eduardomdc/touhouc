#include "packets.h"
#include <arpa/inet.h>
#include <stdio.h>
#include "server.h"
#include "../player.h"
#include "../compact_array.h"
#include "../bullets.h"
#include <string.h>

PacketBuffer packetBuffer = {0};

void sendTcpPlayerHit(Team tplayer){
    TcpHeader tcpHeader = {TCP_PLAYER_HIT};
    TcpPlayerHit tcpPlayerHit = {tplayer};
    send(gameServer.clientTCPSock, &tcpHeader, sizeof(tcpHeader), 0);
    send(gameServer.clientTCPSock, &tcpPlayerHit, sizeof(tcpPlayerHit), 0);
}

void receiveTcpPlayerHit(TcpPlayerHit tcpPlayerHit){
    PlaySound(assets.soundEffects[PLAYER_HIT]);
}

void sendTcpPlayerData(Team tplayer, Player player){
    TcpHeader tcpHeader = {TCP_PLAYER_DATA};
    TcpPlayerData data = {tplayer, player};
    send(gameServer.clientTCPSock, &tcpHeader, sizeof(tcpHeader), 0);
    send(gameServer.clientTCPSock, &data, sizeof(data), 0);
}

void receiveTcpPlayerData(TcpPlayerData tcpPlayerData){
    player = tcpPlayerData.player;
}

void sendTcpPlayerItemPickup(Team tplayer, ItemType itemType){

}

void sendUDPBulletArray(Team team) {
    resetPacketBuffer();
    UdpHeader udpHeader;
    udpHeader.packetType = UDP_BULLET_ARRAY;
    writePacketBuffer(&udpHeader, sizeof(UdpHeader));

    UdpBulletArray udpBulletArray;
    udpBulletArray.team = team;
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

void receiveUDPBulletArray() {
    UdpBulletArray udpBulletArray;
    readPacketBuffer(&udpBulletArray, sizeof(udpBulletArray));
    switch (udpBulletArray.team){
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

void sendUDPPlayerData(Team tplayer){
    UdpHeader udpHeader;
    udpHeader.packetType = UDP_PLAYER_DATA;
    resetPacketBuffer();
    writePacketBuffer(&udpHeader, sizeof(UdpHeader));
    
    UdpPlayerData udpPlayerData;
    udpPlayerData.player = tplayer;
    writePacketBuffer(&udpPlayerData, sizeof(UdpPlayerData));
    
    writePacketBuffer(&player, sizeof(Player));

    socklen_t addrlen = sizeof(gameServer.clientAddress);
    if (sendto(gameServer.udpSock, packetBuffer.bytes, packetBuffer.len, 0, (struct sockaddr*)&gameServer.clientAddress, addrlen) < 0){
        fprintf(stderr, "Failed to send UDP player data\n");
    };
}

void sendUDPPlayerFire(){
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
