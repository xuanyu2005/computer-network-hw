#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 4096

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <command>" << std::endl;
        std::cerr << "Commands: LIST or <filename>" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    int client_socket;
    struct sockaddr_in server_addr;

    // 创建 socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // 配置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 固定连接127.0.0.1

    // 连接服务器
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        close(client_socket);
        return 1;
    }

    std::cout << "Connected to server: 127.0.0.1:" << PORT << std::endl;

    if (command == "LIST") {
        // 发送 LIST 命令
        send(client_socket, command.c_str(), command.size() + 1, 0);

        // 接收文件列表
        char buffer[BUFFER_SIZE];
        std::string file_list;

        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            std::cerr << "Failed to receive file list" << std::endl;
            close(client_socket);
            return 1;
        }
        file_list = std::string(buffer, 0, bytes_received);

        std::cout << "Available files on server:" << std::endl;
        std::cout << file_list << std::endl;
    } else {
        // 发送文件名
        send(client_socket, command.c_str(), command.size() + 1, 0);

        // 接收文件内容
        std::ofstream file(command, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Failed to create file: " << command << std::endl;
            close(client_socket);
            return 1;
        }

        char buffer[BUFFER_SIZE];
        while (true) {
            int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
            if (bytes_received <= 0) {
                break;
            }
            file.write(buffer, bytes_received);
        }

        file.close();
        std::cout << "File received successfully: " << command << std::endl;
    }

    close(client_socket);
    return 0;
}