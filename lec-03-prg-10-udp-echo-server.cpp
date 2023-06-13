#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
using namespace std;

const char HOST[10] = "127.0.0.1";
const int PORT = 65456;
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
using namespace std;


int main()
{
    int serverSocket;

    cout << "> echo-server is activated" << endl;

    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == -1)
    {
        cout << "> create socket failed and program terminated" << endl;
        return -1;
    }

    sockaddr_in address;
    socklen_t addrlen;
    address.sin_family = AF_INET;

    try
    {
        inet_pton(AF_INET, HOST, &address.sin_addr.s_addr);
        address.sin_port = htons(PORT);

        addrlen = sizeof(address);

        int bindFlag = bind(serverSocket, (sockaddr *)&address, addrlen);
        if (bindFlag == -1)
        {
            cout << "> bind() failed and program terminated" << endl;
            close(serverSocket);
            return -1;
        }
    }
    catch (exception e)
    {
        cout << "> bind() failed by exception:" << endl;
        close(serverSocket);
        return -1;
    }

    char buffer[1024] = {0};
    while (1)
    {
        sockaddr_in clientAddress;
        socklen_t clientAddress_len = sizeof(clientAddress);
        memset(&clientAddress, 0, sizeof(clientAddress));
        memset(buffer, 0, sizeof(buffer));

        int sz = recvfrom(serverSocket, buffer, 1024, 0, (sockaddr *)&clientAddress, &clientAddress_len);
        cout << "> echoed: " << buffer << endl;
        sendto(serverSocket, buffer, strlen(buffer), 0, (sockaddr *)&clientAddress, clientAddress_len);
    }
    std::cout << "> echo-server is de-activated" << endl;
    return 0;
}
