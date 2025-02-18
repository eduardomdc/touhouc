#include "server.h"
#include <stdio.h>
#include <fcntl.h>

Server gameServer = {0};

void initServer(){
    // TCP SERVER
    if ((gameServer.serverTCPSock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Failed to create server socket\n");
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

/*
    int server_fd, new_socket;
    ssize_t valread;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    char* hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket
         = accept(server_fd, (struct sockaddr*)&address,
                  &addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("aaa");
    valread = read(new_socket, buffer,
                   1024 - 1); // subtract 1 for the null
                              // terminator at the end
    printf("%s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");

    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    close(server_fd);
    return 0;
*/