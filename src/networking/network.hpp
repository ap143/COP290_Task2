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
#include "../utils/helper.hpp"

class Client
{
private:
    // Buffer length of messages
    const static int BUFFER_WIDTH = 1024;

    // True if the socket is active
    bool active;

    // IP address of the connection
    std::string ip;

    // Port of the connection
    int port;

    // Char buffer to send
    char buffer[Client::BUFFER_WIDTH];

    // Socket
    int sock;

    // Socket properties
    struct sockaddr_in serv, client;
    socklen_t l, m;

public:
    // Constructor
    Client(std::string ip);

    // Function to send messages to the server
    void send(std::string message);

    // Function to recieve new message from the server
    std::string get();

    // Function to close the socket
    void end();

    // Returns true if the socket is active
    bool isActive();
};

class Server
{
private:
    // Buffer length of messages
    const static int BUFFER_WIDTH = 1024;

    // True if the socket is active
    bool active;

    // IP address of the connection
    std::string ip;
    
    // Port of the connection
    int port;

    // Char buffer to send
    char buffer[Server::BUFFER_WIDTH];

    // Socket
    int sock;

    // Socket properties
    struct sockaddr_in serv, client;
    socklen_t l;

public:
    // Constructor
    Server(std::string ip);
    
    // Function to send messages to the server
    void send(std::string message);
    
    // Function to recieve new message from the server
    std::string get();
    
    // Function to close the socket
    void end();

    // Returns true if the socket is active
    bool isActive();
};

#endif