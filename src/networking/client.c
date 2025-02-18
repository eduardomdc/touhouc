#include "client.h"
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "server.h"

#define SERVER_IP "127.0.0.1"

Client gameClient = {0};

void initClient(){
    if ((gameClient.clientTCPSock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Failed to create client tcp socket\n");
        return;
    }
    gameClient.serverAddress.sin_family = AF_INET;
    gameClient.serverAddress.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &gameClient.clientAddress.sin_addr);
    int status;
    if ((
        status = connect(
            gameClient.clientTCPSock, 
            (struct sockaddr*)&gameClient.serverAddress,
            sizeof(gameClient.serverAddress)
            )
        ) < 0) {
        fprintf(stderr, "Failed to connect to server\n");
        return;
   }

   fprintf(stderr, "Connected to server!\n");
}

void closeClient(){
    close(gameClient.clientTCPSock); // close socket
}