#include "packets.h"
#include <arpa/inet.h>
#include <stdio.h>
#include "server.h"
#include "../player.h"
#include "../compact_array.h"
#include "../bullets.h"

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

void sendUDPBulletArray() {
    UdpHeader udpHeader;
    udpHeader.packetType = UDP_BULLET_ARRAY;
    udpHeader.seq = 0;
    udpHeader.len = compactEnemyBulletArray.freeIndex;
    socklen_t addrlen = sizeof(gameServer.clientAddress);
    if (sendto(gameServer.udpSock, &udpHeader, sizeof(udpHeader), 0, (struct sockaddr*)&gameServer.clientAddress, addrlen) < 0){
        fprintf(stderr, "Failed to send UDP header\n");
    }
    if (sendto(gameServer.udpSock, &compactEnemyBulletArray.array, sizeof(Bullet)*udpHeader.len, 0, (struct sockaddr*)&gameServer.clientAddress, addrlen) < 0){
        fprintf(stderr, "Failed to send UDP bullet data\n");
    };
}

void receiveUDPBulletArray() {

}