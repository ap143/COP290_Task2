#include "network.hpp"

Server::Server(std::string ip)
{
    this->ip = ip;
    this->port = 53000;

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = inet_addr(ip.c_str());

    if (bind(sock, (struct sockaddr *)&serv, sizeof(serv)) < 0)
    {
        error("Error binding socket...");
    }

    l = sizeof(client);
}

void Server::send(std::string message)
{
    if(sendto(sock, message.c_str(), message.length(), 0, (struct sockaddr *)&client, l) < 0)
    {
        std::cout << "Cannot send message..." << std::endl;
    }
}

std::string Server::get()
{
    memset(buffer, 0, sizeof(buffer));
    int bytesRecieved = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&client, &l);
    return std::string(buffer, bytesRecieved);
}

void Server::end()
{
    close(sock);
}
