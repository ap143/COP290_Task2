#ifndef NET
#define NET

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <ifaddrs.h>
#include "utils/helper.hpp"

class Client
{
private:

    const static int BUFFER_WIDTH = 1024;

    std::string ip;
    int port;
    char buffer[Client::BUFFER_WIDTH];

    int sock;
    struct sockaddr_in serv, client;
    socklen_t l, m;

public:

    Client(std::string ip);
    void send(std::string message);
    std::string get();
    void end();

};

class Server
{
private:

    const static int BUFFER_WIDTH = 1024;

    std::string ip;
    int port;
    char buffer[Server::BUFFER_WIDTH];

    int sock;
    struct sockaddr_in serv, client;
    socklen_t l;

public:

    Server(std::string ip);
    void send(std::string message);
    std::string get();
    void end();

};

#endif