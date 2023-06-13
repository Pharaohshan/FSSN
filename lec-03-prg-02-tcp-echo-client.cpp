#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <iostream>
using namespace std;

const char HOST[10] = "127.0.0.1";
const int PORT = 65456;

struct sockaddr_in client_addr;
socklen_t length = sizeof(client_addr);

int main()
{
    int clientSocket;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    cout << "> echo-client is activated" << endl;

    sockaddr_in address;
    address.sin_family = AF_INET;
    inet_pton(AF_INET, HOST, &address.sin_addr.s_addr);
    address.sin_port = htons(PORT);
    socklen_t addrlen = sizeof(address);

    connect(clientSocket, (sockaddr *)&address, addrlen);
    char buffer[1024];
    while(true)
    {
        cout << "> ";
        cin.getline(buffer, 1024);
        send(clientSocket, buffer, sizeof(buffer), 0);
        recv(clientSocket, buffer, sizeof(buffer), 0);
        cout << "> received:" << buffer << endl;
        if (strcmp(buffer, "quit") == 0)
            break;
    }

    cout << "> echo-client is de-activated" << endl;
    return 0;
}