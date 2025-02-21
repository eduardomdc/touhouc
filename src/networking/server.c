#include "server.h"
#include "../player.h"
#include "../bullets.h"
#include "packets.h"
#include <stdio.h>
#include <fcntl.h>

Server gameServer = {0};

void initServer(){
    gameServer.bulletPacketTimer = createTimer(0.1);
    gameServer.playerPacketTimer = createTimer(0.02);

    // TCP SERVER
    if ((gameServer.serverTCPSock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Failed to create server tcp socket\n");
        return;
    }

    int opt = 1;
    if (setsockopt(
        gameServer.serverTCPSock, 
        SOL_SOCKET,
        SO_REUSEADDR, // dont wait for os to free address
        &opt,
        sizeof(opt))) {
        fprintf(stderr, "Failed to set server socket options\n");
        return;
    }

    //set non-blocking mode, server can listen for new client connections without blocking game
    int flags = fcntl(gameServer.serverTCPSock, F_GETFL, 0);
    fcntl(gameServer.serverTCPSock, F_SETFL, flags | O_NONBLOCK);

    gameServer.serverAddress.sin_family = AF_INET;
    gameServer.serverAddress.sin_addr.s_addr = INADDR_ANY; // all interfaces
    gameServer.serverAddress.sin_port = htons(PORT);
    socklen_t addrlen = sizeof(gameServer.serverAddress);
    if (bind(gameServer.serverTCPSock, (struct sockaddr*)&gameServer.serverAddress, addrlen) < 0) {
        fprintf(stderr, "Failed to bind server socket to address\n");
        return;
    }

    if (listen(gameServer.serverTCPSock, MAX_CONNECTION_QUEUE) < 0) {
        fprintf(stderr, "Failed to set listen to tcp server socket\n");
        return;
    }

    // UDP Server
    if ((gameServer.udpSock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        fprintf(stderr, "Failed to create server udp socket\n");
        return;
    }
    //set non-blocking mode
    flags = fcntl(gameServer.udpSock, F_GETFL, 0);
    fcntl(gameServer.udpSock, F_SETFL, flags | O_NONBLOCK);

    gameServer.active = true;
}

void serverCheckForClientConnection(){
    socklen_t addrlen = sizeof(gameServer.serverAddress);
    struct sockaddr_in tempAddr;
    if ((
        gameServer.clientTCPSock = accept(
            gameServer.serverTCPSock, 
            (struct sockaddr*)&tempAddr,
            &addrlen
        )) 
    >= 0) {
        gameServer.clientAddress.sin_family = AF_INET;
        gameServer.clientAddress.sin_port = htons(PORT);
        gameServer.clientAddress.sin_addr = tempAddr.sin_addr;
        gameServer.clientIsConnected = true;
        fprintf(stderr, "Client connected!\n");
        sendTcpPlayerData(MARISA, players[MARISA]);
    }
   
}

void sendGameUpdate(){
    updateTimer(&gameServer.bulletPacketTimer);
    updateTimer(&gameServer.playerPacketTimer);
    if (gameServer.bulletPacketTimer.ready){
        sendUDPBulletArray(TEAM_ENEMY);
        sendUDPBulletArray(TEAM_PLAYERS);
        sendUDPEnemyData();
        sendUDPItemData();
        resetTimer(&gameServer.bulletPacketTimer);
    }
    if (gameServer.playerPacketTimer.ready){
        sendUDPPlayerData(MARISA);
        resetTimer(&gameServer.playerPacketTimer);
    }
}

void serverReceiveUdp(){
    if (!gameServer.clientIsConnected) return;
    UdpHeader header;
    socklen_t addrlen = sizeof(gameServer.clientAddress);
    while(
        (recvfrom(gameServer.udpSock, packetBuffer.bytes, packetBuffer.len, 0, (struct sockaddr*)&gameServer.clientAddress, addrlen)) > 0
    ){
        resetPacketBuffer();
        readPacketBuffer(&header, sizeof(UdpHeader));
        switch (header.packetType) {
            case UDP_INPUT_DATA:
                receiveUDPInputData();
            default:
                return;
        }
    }
}