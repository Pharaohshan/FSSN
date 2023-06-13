# FSSN
For this project, I chose Socket Programming for the content shown in the fourth lesson.

1.Why?
a.Network communication: Socket technology provides a reliable way to establish network communication between computers. Using sockets, connections can be established between different computers, enabling data transfer. This allows developers to create various network applications such as chat applications, remote control applications, file transfer applications, and more.
b.Cross-platform compatibility: Socket technology is cross-platform and can be used on almost all major operating systems. This means that network programming can be done using the same socket interfaces and protocols on Windows, Mac, Linux, and other systems. This cross-platform compatibility allows developers to write code once and deploy and run network applications on different platforms.
c.Support for multiple communication protocols: Socket technology supports multiple communication protocols, such as TCP and UDP. TCP provides reliable, connection-oriented communication suitable for applications that require data integrity and order. UDP is a connectionless communication protocol suitable for applications with high latency requirements and acceptable data loss. Using sockets, developers can choose the communication protocol that best suits their application needs.
d.Flexibility and scalability: Socket technology provides flexibility and scalability, allowing developers to customize based on their application requirements. Through socket programming, various network communication models such as client-server models and peer-to-peer models can be implemented. Developers can design and implement their own network protocols, message formats, and data exchange mechanisms based on specific needs.

In summary, socket technology offers a reliable, cross-platform, flexible, and scalable mechanism for implementing network communication between computers. It serves as the foundation for building various network applications and is widely used in the Internet and local area networks.


2.How?
I am going to refactor and implement in c++ based on the practical code covered in the ppt content. Using C++ for Socket programming offers advantages such as high performance, wide support, better control capability, portability, efficiency, and flexibility. 

3.TODO
C++ socket API based on Linux
1. First we need to #include the header file
When we do socket programming, we first #include <sys/types.h> and #include <sys/socket.h> as necessary header files for socket functions, etc. We also need some structures to store information such as ip addresses, so we Also #include <netinet/in.h> to create socket
We generally use the socket() function to create a socket, and the usage example is as follows
socket(DOMAIN, SOCKET_TYPE, PROTOCOL);
2. Create ip/port structure
To access the other party, you must have the other party's ip and port, so we need to create an address-related structure
struct sockaddr_in server_address;
Set the address group (ipv4) for this address
server_address.sin_family = AF_INET;
Set the port for this address
server_address.sin_port = htons(9002);
Set the ip of the peer's address
server_address.sin_addr.s_addr = INADDR_ANY;
3. connect server
connect(SOCKET, ADDR, ADDR_LEN);
4. bind
bind(SOCKET, struct sockaddr* ADDR, ADDR_LEN);
5. listen
listen(socket, backlog);
6. accept
accept(sockfd, struct sockaddr*, addrlen)

Each file corresponds to the implementation idea of c++
1. For the simple implementation of lec-03-prg-01-tcp-echo-server.py and lec-03-prg-02-tcp-echo-client.py, you can directly use the corresponding api.
2. For lec-03-prg-03-tcp-echo-server-complete.py and lec-03-prg-04-tcp-echo-client-complete.py, you can use c++ exception mechanism, try and catch statements accomplish.
3. For lec-03-prg-06-tcp-echo-server-multithread.py and lec-03-prg-07-tcp-echo-client-multithread.py, use the c++11 standard multithread library thread to fulfill.
4. For lec-03-prg-08-tcp-echo-server-multithread-chat.py, it can be realized by changing the code logic on the basis of 
5. For the part that uses the socketserver library in python, it is simply simulated in c++ with class.

