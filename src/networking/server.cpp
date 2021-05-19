#include "network.hpp"

// Constructor
Server::Server(std::string ip)
{
    this->ip = ip;
    this->port = 53000;

    // Initialise socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    // Set the connection properties
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = inet_addr(ip.c_str());

    // Bind the socket
    if (bind(sock, (struct sockaddr *)&serv, sizeof(serv)) < 0)
    {
        error("Error binding socket...");
    }

    // Set active true
    active = true;

    // Set client size
    l = sizeof(client);
}

// Function to send messages to the server
void Server::send(std::string message)
{
    // Try sending the message
    if(sendto(sock, message.c_str(), message.length(), 0, (struct sockaddr *)&client, l) < 0)
    {
        // Print the error
        error("Cannot send message...");
    }
}

// Function to recieve new message from the server
std::string Server::get()
{
    // Reset the buffer
    memset(buffer, 0, sizeof(buffer));
    
    // Recieve the messages
    int bytesRecieved = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&client, &l);
    
    // Return the message
    return std::string(buffer, bytesRecieved);
}

// Function to close the socket
void Server::end()
{
    // Set the active false
    active = false;

    // Close the socket
    close(sock);
}

// Returns true if the socket is active
bool Server::isActive()
{
    return active;
}
