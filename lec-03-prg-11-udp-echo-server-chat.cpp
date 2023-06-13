#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <algorithm>
using namespace std;

const char HOST[10] = "127.0.0.1";
const int PORT = 65456;
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <vector>
using namespace std;


std::vector<sockaddr_in> group_queue;

bool operator==(sockaddr_in a, sockaddr_in b)
{
    if(a.sin_addr.s_addr == b.sin_addr.s_addr && a.sin_family == b.sin_family && a.sin_port == b.sin_port)
        return true;
    else
        return false;
}

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
    char clientIPBuffer[64];
    memset(clientIPBuffer, 0, sizeof(clientIPBuffer));
    while (1)
    {
        sockaddr_in clientAddress;
        socklen_t clientAddress_len = sizeof(clientAddress);
        memset(&clientAddress, 0, sizeof(clientAddress));
        memset(buffer, 0, sizeof(buffer));

        int sz = recvfrom(serverSocket, buffer, 1024, 0, (sockaddr *)&clientAddress, &clientAddress_len);
        int clientPORT = ntohs(clientAddress.sin_port);
        inet_ntop(AF_INET, &clientAddress.sin_addr, clientIPBuffer, sizeof(clientAddress));

        if (buffer[0] == '#' || strcmp(buffer, "quit") == 0)
        {
            if(strcmp(buffer, "#REG") == 0)
            {
                cout << "> client registered " << clientPORT << ", " << clientIPBuffer << endl;
                group_queue.push_back(clientAddress);
            }
            else if(strcmp(buffer, "#DEREG") == 0 || strcmp(buffer, "quit") == 0)
            {
                auto index = std::find(group_queue.begin(), group_queue.end(), clientAddress);
                if(index != group_queue.end())
                {
                    cout << "> client de-registered " << clientPORT << ", " << clientIPBuffer << endl;
                    group_queue.erase(index);
                }
            }
        }
        else
        {
            if(group_queue.size() == 0)
            {
                cout << "> no clients to echo" << endl;
            }
            else if (std::find(group_queue.begin(), group_queue.end(), clientAddress) == group_queue.end())
            {
                cout << "> ignores a message from un-registered client" << endl;
            }
            else
            {
                cout << "> received ( " << buffer << " ) and echoed to  " << group_queue.size() << " clients" << endl;
                for(auto clientAddress: group_queue)
                {
                    sendto(serverSocket, buffer, sizeof(buffer), 0, (sockaddr *)&clientAddress, sizeof(clientAddress));
                }
            }
        }
    }
    std::cout << "> echo-server is de-activated" << endl;
    return 0;
}
