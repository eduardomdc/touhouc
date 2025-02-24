#ifndef SERVER_H
#define SERVER_H
#include <netinet/in.h>
#include <raylib.h>
#define TCP_PORT 27015
#define UDP_PORT 8080
#define MAX_CONNECTION_QUEUE 5

#include "../timer.h"

typedef struct Server {
    int serverTCPSock;
    int clientTCPSock;
    int udpSock;
    struct sockaddr_in tcpServerAddress;
    struct sockaddr_in udpServerAddress;
    struct sockaddr_in tcpClientAddress;
    struct sockaddr_in udpClientAddress;
    Timer bulletPacketTimer;
    Timer playerPacketTimer;
    bool active; // server has been setup and is looking for connections
    bool clientIsConnected; // server has connected client and now sends data to him
} Server;

extern Server gameServer;

void initServer();
void serverCheckForClientConnection();
void serverReceiveUdp();
void sendGameUpdate();

#endif