#include "tcpsocket.h"
#include <iostream>
#include <string>
using namespace std;

int main()
{
    int client_fd = createClientSocket();
    if (client_fd < 0)
    {
        cerr << "Failed to create client socket" << endl;
        return -1;
    }

    cout << "Connected to server. Type '?' to exit." << endl;

    while (true)
    {
        char buffer[1024] = {0};
        string input;

        cout << "send> ";
        getline(cin, input);

        // 检查是否输入了退出命令
        if (input == "?")
        {
            cout << "Exiting connection..." << endl;
            break;
        }

        // 将输入转换为字符数组
        strcpy(buffer, input.c_str());

        if (send(client_fd, buffer, strlen(buffer), 0) < 0)
        {
            perror("send error");
            break;
        }
        else
        {
            cout << "Message sent: " << buffer << endl;
        }

        int bytes_received = recv(client_fd, buffer, 1024, 0);
        if (bytes_received > 0)
        {
            cout << "Message received: " << buffer << endl;
        }
        else if (bytes_received == 0)
        {
            cout << "Connection closed by server." << endl;
            break;
        }
        else
        {
            perror("recv error");
            break;
        }
    }

    close(client_fd);
    cout << "---------client end-----------" << endl;
    return 0;
}