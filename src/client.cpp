#include "network.hpp"

void client(int argc, char* argv[])
{

    // Create a socket

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0)
    {
        std::cerr << "Error creating socket..." << std::endl;
        exit(-1);
    }

    // Create a hint structure for the server we are connecting to

    int port = 54000;

    std::string ipAddress = "127.0.0.1";

    sockaddr_in hint;

    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    // Connect to the server on the socket

    int connectRes = connect(sock, (sockaddr*) &hint, sizeof(hint));

    if (connectRes < 0)
    {
        std::cerr << "Error connecting to the server..." << std::endl;
        exit(-1);
    }

    // While loop

    char buf[4096];
    std::string usr_input;

    do
    {
        // Enter lines of text
        std::cout << "< ";
        std::getline(std::cin, usr_input);

        // Send to server
        int sendRes = send(sock, usr_input.c_str(), usr_input.size()+1, 0);

        if (sendRes < 0)
        {
            std::cerr << "Could not send to server... " << std::endl;
            continue;
        }

        // Wait for response
        memset(buf, 0, sizeof(buf));
        int bytesRecieved = recv(sock, buf, sizeof(buf), 0);

        // Display response
        std::cout << "Server> " << std::string(buf, bytesRecieved) << std::endl;
        
    } while (true);
    
    // Close the socket
    close(sock);

}