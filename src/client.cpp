#include "network.hpp"

Client::Client(std::string ip)
{
    this->ip = ip;
    this->port = 53000;

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = inet_addr(ip.c_str());

    l = sizeof(client);
    m = sizeof(serv);

    active =  true;
}

void Client::send(std::string message)
{
    sendto(sock, message.c_str(), message.length(), 0, (struct sockaddr *)&serv, m);
}

std::string Client::get()
{
    memset(buffer, 0, sizeof(buffer));
    int bytesRecieved = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&client, &l);
    return std::string(buffer, bytesRecieved);
}

void Client::end()
{
    active = false;
    close(sock);
}

bool Client::isActive()
{
    return active;
}
