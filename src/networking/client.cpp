#include "network.hpp"

// Constructor
Client::Client(std::string ip)
{
    this->ip = ip;
    this->port = 53000;

    // Create the socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    // Set connection properties
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = inet_addr(ip.c_str());

    // Set size of client and server
    l = sizeof(client);
    m = sizeof(serv);

    // Assert active
    active =  true;
}

// Function to send messages to the server
void Client::send(std::string message)
{
    // Try sending the message
    if (sendto(sock, message.c_str(), message.length(), 0, (struct sockaddr *)&serv, m) < 0)
    {
        // Print error if message is not sent
        error("Cannot send message...");
    }
}

// Function to recieve new message from the server
std::string Client::get()
{
    // Reset the buffer
    memset(buffer, 0, sizeof(buffer));
    
    // Recieve the message
    int bytesRecieved = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&client, &l);
    
    // Return the message
    return std::string(buffer, bytesRecieved);
}

// Function to close the socket
void Client::end()
{
    // Set the active false
    active = false;

    // Close the socket
    close(sock);
}

// Returns true if the socket is active
bool Client::isActive()
{
    return active;
}
