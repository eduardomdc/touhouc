#ifndef CLIENT_H
#define CLIENT_H
#include <netinet/in.h>
#include <raylib.h>

typedef struct Client {
    int tcpSock;
    int udpSock;
    struct sockaddr_in tcpServerAddress;
    struct sockaddr_in udpServerAddress;
    struct sockaddr_in clientAddress;
    bool connected; // is connected to a server
} Client;

extern Client gameClient;

void initClient(char* ipStr);

void clientReceiveTcp();
void clientReceiveUdp();

void closeClient();

#endif