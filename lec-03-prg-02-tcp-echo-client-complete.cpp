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

int main()
{
    int clientSocket;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        cout << "> create socket failed and program terminated" << endl;
        return -1;
    }

    cout << "> echo-client is activated" << endl;

    try
    {
        sockaddr_in address;
        address.sin_family = AF_INET;
        inet_pton(AF_INET, HOST, &address.sin_addr.s_addr);
        address.sin_port = htons(PORT);
        socklen_t addrlen = sizeof(address);

        int connFlag = connect(clientSocket, (sockaddr *)&address, addrlen);
        if (connFlag == -1)
        {
            cout << "> connect() failed and program terminate" << endl;
            close(clientSocket);
            return -1;
        }
    }
    catch(exception e)
    {
        cout << "> connect() failed by exception:" << endl;
        close(clientSocket);
        return -1;
    }

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