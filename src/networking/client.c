#include "client.h"
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "server.h"
#include "../spawner.h"
#include "packets.h"

#define SERVER_IP "127.0.0.3"

Client gameClient = {0};

void initClient(){
    if ((gameClient.tcpSock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Failed to create client tcp socket\n");
        return;
    }
    gameClient.serverAddress.sin_family = AF_INET;
    gameClient.serverAddress.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &gameClient.serverAddress.sin_addr);
    
    int status;
    if ((
        status = connect(
            gameClient.tcpSock, 
            (struct sockaddr*)&gameClient.serverAddress,
            sizeof(gameClient.serverAddress)
            )
        ) < 0) {
        fprintf(stderr, "Failed to connect to server\n");
        return;
    }
    //set non-blocking mode
    int flags = fcntl(gameClient.tcpSock, F_GETFL, 0);
    fcntl(gameClient.tcpSock, F_SETFL, flags | O_NONBLOCK);

    //set up UDP sock
    if ((gameClient.udpSock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        fprintf(stderr, "Failed to create client udp socket\n");
        return;
    }
    //set non-blocking mode
    flags = fcntl(gameClient.udpSock, F_GETFL, 0);
    fcntl(gameClient.udpSock, F_SETFL, flags | O_NONBLOCK);
    //set re-use address option
    int opt = 1;
    if (setsockopt(
        gameClient.udpSock, 
        SOL_SOCKET,
        SO_REUSEADDR, // dont wait for os to free address
        &opt,
        sizeof(opt))) {
        fprintf(stderr, "Failed to set server udp socket options\n");
        return;
    }

    gameClient.clientAddress.sin_family = AF_INET;
    gameClient.clientAddress.sin_port = htons(PORT);
    gameClient.clientAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    socklen_t addrlen = sizeof(gameClient.clientAddress);
    if (bind(gameClient.udpSock, (struct sockaddr*)&gameClient.clientAddress, addrlen) < 0) {
        fprintf(stderr, "Failed to bind client udp socket to address\n");
        return;
    }

    fprintf(stderr, "Connected to server!\n");

    gameClient.connected = true;

    printf("IP address is: %s\n", inet_ntoa(gameClient.clientAddress.sin_addr));
    printf("port is: %d\n", (int) ntohs(gameClient.clientAddress.sin_port));
}

void clientReceiveTcp(){
    TcpHeader header;
    while (recv(gameClient.tcpSock, packetBuffer.bytes, MAX_PACKAGE_BUFFER_SIZE, 0) > 0){
        resetPacketBuffer();
        readPacketBuffer(&header, sizeof(TcpHeader));
        switch (header.packetType) {
            case TCP_PLAYER_DATA:
                fprintf(stderr, "Received player data :)\n");
                receiveTcpPlayerData();
                break;
            case TCP_PLAYER_HIT:
                receiveTcpPlayerHit();
                break;
            case TCP_PLAYER_ITEM_PICK_UP:
                receiveTcpPlayerItemPickup();
            case TCP_ENEMY_DEATH:
                receiveTcpEnemyDeath();
            default:
                return;
        }
    }
}

void clientReceiveUdp(){
    UdpHeader header;
    socklen_t addrlen = sizeof(gameClient.serverAddress);
    int bytesRead = 0;
    while(
        (bytesRead = recvfrom(gameClient.udpSock, packetBuffer.bytes, sizeof(packetBuffer.bytes), 0, (struct sockaddr*)&gameClient.serverAddress, &addrlen)) > 0
    ){
        resetPacketBuffer();
        readPacketBuffer(&header, sizeof(UdpHeader));
        switch (header.packetType) {
            case UDP_BULLET_ARRAY:
                if ( bytesRead < (sizeof(UdpHeader)+sizeof(UdpBulletArray)) ){
                    fprintf(stderr,"Received malformed udp bullet array packet %d bytes\n", bytesRead);
                    continue;
                }
                receiveUDPBulletArray();
                break;
            case UDP_PLAYER_DATA:
                if ( bytesRead != (sizeof(UdpHeader)+sizeof(UdpPlayerData)+sizeof(Player)) ){
                    fprintf(stderr,"Received malformed udp player data packet %d bytes\n", bytesRead);
                    continue;
                }
                receiveUDPPlayerData();
                break;
            case UDP_PLAYER_FIRE:
                if ( bytesRead != (sizeof(UdpHeader)) ){
                    fprintf(stderr,"Received malformed udp player fire packet %d bytes\n", bytesRead);
                    continue;
                }
                receiveUDPPlayerFire();
                break;
            case UDP_ENEMY_DATA:
                if ( bytesRead != (sizeof(UdpHeader)+MAX_ENEMIES*sizeof(Enemy)) ){
                    fprintf(stderr,"Received malformed udp enemy data packet %d bytes\n", bytesRead);
                    continue;
                }
                receiveUDPEnemyData();
                break;
            case UDP_ITEM_DATA:
                if ( bytesRead < (sizeof(UdpHeader)+sizeof(UdpItemData)) ){
                    fprintf(stderr,"Received malformed udp item data packet %d bytes\n", bytesRead);
                    continue;
                }
                receiveUDPItemData();
                break;
            default:
                return;
        }
    }
}

void closeClient(){
    close(gameClient.tcpSock); // close socket
    close(gameClient.udpSock);
}
