#include "client.h"
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "server.h"
#include "packets.h"

#define SERVER_IP "127.0.0.1"

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

    fprintf(stderr, "Connected to server!\n");

    gameClient.connected = true;
}

void clientReceiveTcp(){
    TcpHeader header;
    if (recv(gameClient.tcpSock, &header, sizeof(header), 0) > 0){
        switch (header.packetType) {
            case TCP_PLAYER_DATA:
                fprintf(stderr, "Received player data :)\n");
                TcpPlayerData tcpPlayerData;
                recv(gameClient.tcpSock, &tcpPlayerData, sizeof(tcpPlayerData), 0);
                receiveTcpPlayerData(tcpPlayerData);
                break;
            default:
                return;
        }
    }

}

void closeClient(){
    close(gameClient.tcpSock); // close socket
}