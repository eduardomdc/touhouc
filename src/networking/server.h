#ifndef SERVER_H
#define SERVER_H
#include <netinet/in.h>
#include <raylib.h>
#define PORT 27015
#define MAX_CONNECTION_QUEUE 5

typedef struct Server {
    int serverTCPSock;
    int clientTCPSock;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    bool active; // server has been setup and is looking for connections
    bool clientIsConnected; // server has connected client and now sends data to him
} Server;

extern Server gameServer;

void initServer();
void serverCheckForClientConnection();

#endif