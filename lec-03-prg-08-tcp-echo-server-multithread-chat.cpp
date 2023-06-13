#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <vector>
#include <future>
#include <fcntl.h>
#include <mutex>

using namespace std;

const char HOST[10] = "127.0.0.1";
const int PORT = 65456;

int handleThreadNum = 0;
vector<std::thread::id> handleThreadsStatus; // 一个缓冲区，存储需要退出的handle进程
vector<std::thread *> handleThreads;
vector<int> clientSockets;
bool acceptClientSocketThreadFlag = true;

// 锁
std::mutex handleMutex;
std::mutex flagMutex;

void sendall(int thisClientSocket, char* buffer)
{
    for(int clientSocket: clientSockets)
    {
        if(clientSocket != thisClientSocket)
        {
            send(clientSocket, buffer, strlen(buffer), 0);
        }
    }
}

// clientSocket接收消息线程函数
void handle(int clientSocket)
{
    cout << "client connected by IP address " << HOST << " with Port number " << PORT << endl << "> ";
    flush(cout);
    thread::id cur_thread = this_thread::get_id();
    char buffer[1024];
    while(true)
    {
        memset(buffer, 0, sizeof(buffer));
        int flag = read(clientSocket, buffer, 1024);
        if(flag == -1)
        {
            cout << "> read Error!" << endl;
            continue;
        }
        cout << "echoed: " << buffer << " by " << cur_thread << endl << "> ";
        sendall(clientSocket, buffer);
        flush(cout);
        if(strcmp(buffer, "quit") == 0)
        {
            handleMutex.lock();
            handleThreadsStatus.push_back(cur_thread); // 将此线程的id存入缓冲区
            handleMutex.unlock();
            break;
        }
    }
}

// 和ClientSocket建立连接, 接收消息过程创建新线程
void acceptClientSocket(int serverSocket)
{
    int clientSocket;
    while(true)
    {
        // 处理需要退出的线程
        handleMutex.lock();
        for (int i = 0; i < handleThreadsStatus.size(); i++)
        {
            std::thread::id index = handleThreadsStatus[i];
            for(int j = 0; j < handleThreads.size(); j++)
            {
                if(index == handleThreads[j]->get_id())
                {
                    std::thread *theThread = handleThreads[j];
                    theThread->join();
                    delete theThread;
                    handleThreads.erase(handleThreads.begin() + j);
                    handleThreadNum--;
                    clientSockets.erase(clientSockets.begin() + j);
                    break;
                }
            }
        }
        handleThreadsStatus.clear();
        handleMutex.unlock();
        // 判断当前线程是否需要退出
        flagMutex.lock();
        if (acceptClientSocketThreadFlag == false)
        {
            flagMutex.unlock();
            return;
        }
        flagMutex.unlock();
        // 判断是否需要建立连接，非阻塞
        sockaddr_in clientAddress;
        socklen_t clientAddressLen = sizeof(clientAddress);
        clientSocket = accept(serverSocket, (sockaddr *)&clientAddress, &clientAddressLen);
        if(clientSocket == -1)
            continue;
        else
        {
            handleThreads.push_back(new thread(handle, clientSocket));
            handleThreadNum++;
            clientSockets.push_back(clientSocket);
        }
    }
}


int main()
{
    int serverSocket;
    char buffer[1024] = {0};

    cout << "> echo-server is activated" << endl;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
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

    int listenFlag = listen(serverSocket, 1);
    if (listenFlag == -1)
    {
        cout << "> listen() failed and program terminated" << endl;
        close(serverSocket);
        return -1;
    }

    // 设置serverSocket为非阻塞
    int flags = fcntl(serverSocket, F_GETFL, 0);
    fcntl(serverSocket, F_SETFL, flags | O_NONBLOCK);

    // 启动新线程，可与多个client建立连接
    thread acceptClientSocket_Thread(acceptClientSocket, serverSocket);

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        cout << "> ";
        cin.getline(buffer, 1024);
        if (strcmp(buffer, "quit") == 0)
        {
            if(handleThreadNum == 0)
            {

                flagMutex.lock();
                acceptClientSocketThreadFlag = false;
                flagMutex.unlock();
                cout << "> stop procedure started" << endl;
                break;
            }
            else
            {
                cout << "> active threads are remained : " << handleThreadNum << " threads" << endl;
            }
        }
    }
    acceptClientSocket_Thread.join();
    std::cout << "echo-server is de-activated" << endl;
    return 0;
}