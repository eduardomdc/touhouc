#include "server.h"
#include "../player.h"
#include "../bullets.h"
#include "packets.h"
#include <stdio.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.2"
#define CLIENT_IP "127.0.0.3"

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

    gameServer.tcpServerAddress.sin_family = AF_INET;
    gameServer.tcpServerAddress.sin_addr.s_addr = inet_addr(SERVER_IP); // all interfaces
    gameServer.tcpServerAddress.sin_port = htons(TCP_PORT);
    socklen_t addrlen = sizeof(gameServer.tcpServerAddress);
    if (bind(gameServer.serverTCPSock, (struct sockaddr*)&gameServer.tcpServerAddress, addrlen) < 0) {
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
    gameServer.udpServerAddress.sin_family = AF_INET;
    gameServer.udpServerAddress.sin_addr.s_addr = inet_addr(SERVER_IP); // all interfaces
    gameServer.udpServerAddress.sin_port = htons(UDP_PORT);
    addrlen = sizeof(gameServer.udpServerAddress);
    if (bind(gameServer.udpSock, (struct sockaddr*)&gameServer.udpServerAddress, addrlen) < 0) {
        fprintf(stderr, "Failed to bind server udp socket to address\n");
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
    inet_ntop(AF_INET, &gameServer.udpServerAddress.sin_addr, ipAddr, INET_ADDRSTRLEN);
    printf("IP address is: %s\n", ipAddr);
    printf("udp port is: %d\n", (int) ntohs(gameServer.udpServerAddress.sin_port));
    printf("tcp port is: %d\n", (int) ntohs(gameServer.tcpServerAddress.sin_port));
}

void serverCheckForClientConnection(){
    socklen_t addrlen;
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
        gameServer.tcpClientAddress.sin_family = AF_INET;
        gameServer.tcpClientAddress.sin_port = htons(TCP_PORT);
        gameServer.tcpClientAddress.sin_addr.s_addr = inet_addr(CLIENT_IP);
        gameServer.udpClientAddress.sin_family = AF_INET;
        gameServer.udpClientAddress.sin_port = htons(UDP_PORT);
        gameServer.udpClientAddress.sin_addr.s_addr = inet_addr(CLIENT_IP);
        gameServer.clientIsConnected = true;
        fprintf(stderr, "Client connected!\n");
        char ipAddr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &gameServer.tcpClientAddress.sin_addr, ipAddr, INET_ADDRSTRLEN);
        printf("client IP address is: %s\n", ipAddr);
        printf("tcp client port is: %d\n", (int) ntohs(gameServer.tcpClientAddress.sin_port));
        printf("udp client port is: %d\n", (int) ntohs(gameServer.udpClientAddress.sin_port));
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
        sendUDPItemData();
        resetTimer(&gameServer.bulletPacketTimer);
    }
    if (gameServer.playerPacketTimer.ready){
        sendUDPPlayerData(MARISA);
        sendUDPPlayerData(REIMU);
        sendUDPEnemyData();
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
        (bytesRead = recvfrom(gameServer.udpSock, packetBuffer.bytes, MAX_PACKAGE_BUFFER_SIZE, 0, (struct sockaddr*)&sender, &addrlen)) > 0
    ){
        if (sender.sin_addr.s_addr == gameServer.udpServerAddress.sin_addr.s_addr){
            continue; 
        }
        resetPacketBuffer();
        readPacketBuffer(&header, sizeof(UdpHeader));
        switch (header.packetType) {
            case UDP_INPUT_DATA:
                if ( bytesRead != (sizeof(UdpHeader)+sizeof(UdpInputData)) ){
                    char ipAddr[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &sender.sin_addr, ipAddr, INET_ADDRSTRLEN);
                    fprintf(stderr,"from ip %s:%d Received malformed udp input data packet %d bytes, should be %d header.packetType=%d\n", ipAddr, (int) ntohs(sender.sin_port), bytesRead, sizeof(UdpHeader)+sizeof(UdpInputData), header.packetType);
                    // fprintf(stderr, "Packet contents: ");
                    // for (int i = 0; i < bytesRead; i++) {
                    //     fprintf(stderr, "%02x ", packetBuffer.bytes[i]);
                    // }
                    continue;
                }
                receiveUDPInputData();
                break;
            default:
                char ipAddr[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &sender.sin_addr, ipAddr, INET_ADDRSTRLEN);
                fprintf(stderr,"Not identified from ip %s:%d Received malformed packet %d bytes, header.packetType=%d\n", ipAddr, (int) ntohs(sender.sin_port), bytesRead, header.packetType);
                break;
        }
    }
    if (bytesRead < 0) {
        //fprintf(stderr,"server receive udp recvfrom failed\n");
    }
}
