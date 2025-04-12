#include "tcpsocket.h"
#include <iostream>
using namespace std;

// 创建服务器套接字
int createServerSocket()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Failed to create socket");
        return -1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080); // 服务器端口号
    server_addr.sin_addr.s_addr = INADDR_ANY; // 绑定所有网络接口

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Failed to bind socket");
        close(sockfd);
        return -1;
    }

    if (listen(sockfd, 5) < 0) // 最大等待连接数为 5
    {
        perror("Failed to listen on socket");
        close(sockfd);
        return -1;
    }

    cout << "Server is listening on port 8080..." << endl;
    return sockfd;
}

// 创建客户端套接字
int createClientSocket()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Failed to create socket");
        return -1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080); // 服务器端口号
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr); // 服务器IP地址

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Failed to connect to server");
        close(sockfd);
        return -1;
    }

    cout << "Connected to server" << endl;
    return sockfd;
}