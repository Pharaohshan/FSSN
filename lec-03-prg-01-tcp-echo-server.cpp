#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
using namespace std;

const char HOST[10] = "127.0.0.1";
const int PORT = 65456;

int main()
{
    int serverSocket, clientSocket;
    char buffer[1024] = {0};

    cout << "> echo-server is activated" << endl;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address;
    address.sin_family = AF_INET;
    inet_pton(AF_INET, HOST, &address.sin_addr.s_addr);
    address.sin_port = htons(PORT);
    
    socklen_t addrlen = sizeof(address);

    bind(serverSocket, (sockaddr *)&address, addrlen);
    listen(serverSocket, 1);

    sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);
    clientSocket = accept(serverSocket, (sockaddr *)&clientAddress, &clientAddressLen);
    while(clientSocket)
    {
        memset(buffer, 0, sizeof(buffer));
        read(clientSocket, buffer, 1024);
        cout << "> echoed: " << buffer << endl;
        send(clientSocket, buffer, strlen(buffer), 0);
        if(strcmp(buffer, "quit") == 0)
            break;
    }
    std::cout << "> echo-server is de-activated" << endl;

    return 0;
}
