#include "server.h"
#include <stdio.h>
#include <fcntl.h>

Server gameServer = {0};

void initServer(){
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


    gameServer.active = true;
}

void serverCheckForClientConnection(){
    socklen_t addrlen = sizeof(gameServer.serverAddress);
    if ((
            gameServer.clientTCPSock = accept(
                gameServer.serverTCPSock, 
                (struct sockaddr*)&gameServer.clientAddress, 
                &addrlen
            )
        ) >= 0) {
        gameServer.clientIsConnected = true;
        fprintf(stderr, "Client connected!\n");
   }
   
}