#include "packets.h"
#include <arpa/inet.h>
#include "server.h"
#include "../player.h"

void sendTcpPlayerHit(short playerID);

void sendTcpPlayerData(short playerID, Player player){
    TcpHeader tcpHeader = {TCP_PLAYER_DATA};
    TcpPlayerData data = {0, player};
    send(gameServer.clientTCPSock, &tcpHeader, sizeof(tcpHeader), 0);
    send(gameServer.clientTCPSock, &data, sizeof(data), 0);
}

void receiveTcpPlayerData(TcpPlayerData tcpPlayerData){
    player = tcpPlayerData.player;
}

void sendTcpPlayerItemPickup(short playerID, ItemType itemType);