#ifndef CLIENT_H
#define CLIENT_H
#include <netinet/in.h>
#include <raylib.h>

typedef struct Client {
    int clientTCPSock;
    int serverTCPSock;
    struct sockaddr_in clientAddress;
    struct sockaddr_in serverAddress;
    bool connected; // is connected to a server
} Client;

extern Client gameClient;

void initClient();

void clientReceiveTcp();

void closeClient();

#endif