#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <functional>


using namespace std;

class BaseServer
{

};

class TCPServer : public BaseServer
{
private:
    BaseRequestHandler* _Handler;
    sockaddr_in address;
    string _HOST;
    string _PORT;

public:
    inline TCPServer(string HOST, string PORT, BaseRequestHandler* Handler)
    {

    }

    inline void serve_forever()
    {

    }
};

class BaseRequestHandler
{
public:
    virtual inline void operator()() = 0;
};

class MyTCPSocketHandler : public BaseRequestHandler
{
public:
    MyTCPSocketHandler();
    virtual inline void operator()() override { this->handle(); }

private:
    inline void handle()
    {

    }
};