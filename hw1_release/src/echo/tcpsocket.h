#ifndef _TCPSOCKET_H
#define _TCPSOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// 创建服务器套接字
int createServerSocket();

// 创建客户端套接字
int createClientSocket();

#endif // _TCPSOCKET_H