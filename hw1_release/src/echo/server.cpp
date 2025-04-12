#include "tcpsocket.h"
#include <iostream>
#include <string>
using namespace std;

int main()
{
    int server_fd = createServerSocket();
    if (server_fd < 0)
    {
        cerr << "Failed to create server socket" << endl;
        return -1;
    }

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0)
    {
        perror("Failed to accept connection");
        close(server_fd);
        return -1;
    }

    cout << "Client connected: " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << endl;

    while (true)
    {
        char buffer[1024] = {0};
        int bytes_received = recv(client_fd, buffer, 1024, 0);
        if (bytes_received <= 0)
        {
            if (bytes_received < 0)
                perror("recv error");
            else
                cout << "Client disconnected" << endl;
            break;
        }

        cout << "Received: " << buffer << endl;

        string input;
        cout << "send> ";
        getline(cin, input);

        // 检查是否输入了退出命令
        if (input == "?")
        {
            cout << "Shutting down server..." << endl;
            break;
        }

        // 将输入转换为字符数组
        strcpy(buffer, input.c_str());

        if (send(client_fd, buffer, strlen(buffer), 0) < 0)
        {
            perror("send error");
            break;
        }
    }

    close(client_fd);
    close(server_fd);
    cout << "---------server end-----------" << endl;
    return 0;
}