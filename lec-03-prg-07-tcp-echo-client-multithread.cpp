#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <vector>
#include <mutex>
#include <fcntl.h>

using namespace std;

const char HOST[10] = "127.0.0.1";
const int PORT = 65456;

bool recvHandlerFlag = true;
std::mutex flagMutex;

void recvHandler(int clientSocket)
{
    int flags = fcntl(clientSocket, F_GETFL, 0);
    fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK);
    char buffer[1024];
    while(true)
    {
        memset(buffer, 0, sizeof(buffer));
        int flag = read(clientSocket, buffer, 1024);
        if (flag == -1)
        {
            continue;
        }
        cout << "received: " << buffer << endl << "> ";
        flush(cout);
        // 判断是否需要退出
        flagMutex.lock();
        if(recvHandlerFlag == false)
        {
            flagMutex.unlock();
            return;
        }
        flagMutex.unlock();
    }
}

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
    catch (exception e)
    {
        cout << "> connect() failed by exception:" << endl;
        close(clientSocket);
        return -1;
    }

    thread clientThread = thread(recvHandler, clientSocket);

    char buffer[1024];
    while(true)
    {
        cout << "> ";
        flush(cout);
        cin.getline(buffer, 1024);
        send(clientSocket, buffer, sizeof(buffer), 0);
        if (strcmp(buffer, "quit") == 0)
        {
            flagMutex.lock();
            recvHandlerFlag = false;
            flagMutex.unlock();
            break;
        }
    }

    clientThread.join();
    cout << "echo-client is de-activated" << endl;
    return 0;
}