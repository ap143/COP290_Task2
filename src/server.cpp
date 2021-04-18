#include "network.hpp"

std::string getIPAddress(){
    std::string ipAddress="Unable to get IP Address";
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    int success = 0;
    // retrieve the current interfaces - returns 0 on success
    success = getifaddrs(&interfaces);
    if (success == 0) {
        // Loop through linked list of interfaces
        temp_addr = interfaces;
        while(temp_addr != NULL) {
            if(temp_addr->ifa_addr->sa_family == AF_INET) {
                // Check if interface is en0 which is the wifi connection on the iPhone
                if(strcmp(temp_addr->ifa_name, "en0")){
                    ipAddress=inet_ntoa(((struct sockaddr_in*)temp_addr->ifa_addr)->sin_addr);
                }
            }
            temp_addr = temp_addr->ifa_next;
        }
    }
    // Free memory
    freeifaddrs(interfaces);
    return ipAddress;
}

void server(int argc, char* argv[])
{

    std::cout << getIPAddress() << std::endl;

    int listening = socket(AF_INET, SOCK_STREAM, 0);

    if (listening == -1)
    {
        std::cerr << "Can't create socket" << std::endl;
        exit(-1);
    }

    // socket()
    // bind()
    // listen()
    // accept()
    // send()
    // recieve()

    sockaddr_in hint;

    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if (bind(listening,  (sockaddr*) &hint, sizeof(hint)) == -1)
    {
        std::cerr << "Can't bind to IP port" << std::endl;
        exit(-1);
    }

    if (listen(listening, SOMAXCONN) == -1)
    {
        std::cerr << "Can't listen" << std::endl;
        exit(-1);
    }

    sockaddr_in client;
    socklen_t clientSize = sizeof(client);

    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    int clientSocket = accept(listening, (sockaddr*) &client, &clientSize);

    if (clientSocket == -1)
    {
        std::cerr << "Problem with client connnecting" << std::endl;
        exit(-1);
    }

    close(listening);

    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    int result = getnameinfo((sockaddr*) &client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);

    if (result)
    {
        std::cout << "Connected on " << svc << std::endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << "Connected on " << ntohs(client.sin_port) << std::endl;
    }

    char buf[4096];

    while (true)
    {
        memset(buf, 0, sizeof(buf));

        int bytesRecv = recv(clientSocket, buf, sizeof(buf), 0);

        if (bytesRecv == -1)
        {
            std::cerr << "Connection issue" << std::endl;
            break;
        }

        if (bytesRecv == 0)
        {
            std::cout << "The client disconnected" << std::endl;
            break;
        }

        std::cout << "Recieved: " << std::string(buf, 0, bytesRecv) << std::endl;

        send(clientSocket, buf, bytesRecv+1, 0);

    }

    close(clientSocket);

}