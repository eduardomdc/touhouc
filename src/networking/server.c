#include "server.h"
#include "../player.h"
#include "../bullets.h"
#include "packets.h"
#include <stdio.h>
#include <fcntl.h>
#include <arpa/inet.h>

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
    //set re-use address option
    opt = 1;
    if (setsockopt(
        gameServer.udpSock, 
        SOL_SOCKET,
        SO_REUSEADDR, // dont wait for os to free address
        &opt,
        sizeof(opt))) {
        fprintf(stderr, "Failed to set server udp socket options\n");
        return;
    }

    gameServer.active = true;
    char ipAddr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &gameServer.serverAddress.sin_addr, ipAddr, INET_ADDRSTRLEN);
    printf("IP address is: %s\n", ipAddr);
    printf("port is: %d\n", (int) ntohs(gameServer.serverAddress.sin_port));
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
    < 0) {
        return;
    } else {
        gameServer.clientAddress.sin_family = AF_INET;
        gameServer.clientAddress.sin_port = htons(PORT);
        gameServer.clientAddress.sin_addr = tempAddr.sin_addr;
        gameServer.clientIsConnected = true;
        fprintf(stderr, "Client connected!\n");
        players[REIMU].connected = true;
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
        sendUDPPlayerData(REIMU);
        resetTimer(&gameServer.playerPacketTimer);
    }
}

void serverReceiveUdp(){
    if (!gameServer.clientIsConnected) return;
    UdpHeader header = {0};
    
    int bytesRead = 0;
    struct sockaddr_in sender;
    socklen_t addrlen = sizeof(sender);
    while(
        (bytesRead = recvfrom(gameServer.udpSock, packetBuffer.bytes, packetBuffer.len, 0, (struct sockaddr*)&sender, &addrlen)) > 0
    ){
        if (sender.sin_addr.s_addr == gameServer.serverAddress.sin_addr.s_addr){
            continue; 
        }
        resetPacketBuffer();
        readPacketBuffer(&header, sizeof(UdpHeader));
        switch (header.packetType) {
            case UDP_INPUT_DATA:
                if ( bytesRead != (sizeof(UdpHeader)+sizeof(UdpInputData)) ){
                    fprintf(stderr,"Received malformed udp input data packet %d bytes\n", bytesRead);
                    continue;
                }
                receiveUDPInputData();
                break;
            default:
                break;
        }
    }
    if (bytesRead < 0) {
        fprintf(stderr,"server receive udp recvfrom failed\n");
    }
}