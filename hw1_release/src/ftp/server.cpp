#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>

#define PORT 8080
#define MAX_CLIENTS 5
#define BUFFER_SIZE 4096

// 获取当前目录下的文件列表
std::vector<std::string> get_file_list() {
    std::vector<std::string> files;
    DIR* dir;
    struct dirent* entry;

    if ((dir = opendir(".")) != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_REG) { // 只列出普通文件
                files.push_back(entry->d_name);
            }
        }
        closedir(dir);
    }
    return files;
}

void handle_client(int client_socket, const std::string& client_id) {
    char buffer[BUFFER_SIZE];
    std::string command;

    // 接收客户端的命令
    int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received <= 0) {
        std::cerr << "Client " << client_id << ": Failed to receive command" << std::endl;
        close(client_socket);
        return;
    }
    command = std::string(buffer, 0, bytes_received);

    // 去掉命令末尾的换行符
    if (!command.empty() && command.back() == '\n') {
        command.pop_back();
    }

    if (command == "LIST") {
        // 发送文件列表
        std::vector<std::string> files = get_file_list();
        std::string file_list_str;
        for (const auto& file : files) {
            file_list_str += file + "\n";
        }
        send(client_socket, file_list_str.c_str(), file_list_str.size() + 1, 0);
        std::cout << "Client " << client_id << ": Sent file list to client" << std::endl;
    } else {
        // 接收文件名
        std::string filename = command;

        std::cout << "Client " << client_id << ": Receiving file: " << filename << std::endl;

        // 创建文件
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Client " << client_id << ": Failed to create file: " << filename << std::endl;
            close(client_socket);
            return;
        }

        // 接收文件内容
        while (true) {
            bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
            if (bytes_received <= 0) {
                break;
            }
            file.write(buffer, bytes_received);
        }

        file.close();
        std::cout << "Client " << client_id << ": File received successfully: " << filename << std::endl;
    }

    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // 打印当前工作目录
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        std::cout << "Server working directory: " << cwd << std::endl;
    } else {
        std::cerr << "Failed to get current working directory" << std::endl;
    }

    // 创建 socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // 配置 socket 地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 固定监听127.0.0.1

    // 绑定 socket
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(server_socket);
        return 1;
    }

    // 监听连接
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        std::cerr << "Listen failed" << std::endl;
        close(server_socket);
        return 1;
    }

    std::cout << "Server listening on 127.0.0.1:" << PORT << std::endl;

    // 接受客户端连接
    while (true) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }

        // 生成客户端标识符（IP:端口）
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
        std::string client_id = std::string(client_ip) + ":" + std::to_string(ntohs(client_addr.sin_port));

        std::cout << "Client connected: " << client_id << std::endl;

        // 处理客户端请求（多线程）
        if (fork() == 0) {
            close(server_socket);
            handle_client(client_socket, client_id);
            exit(0);
        } else {
            close(client_socket);
        }
    }

    close(server_socket);
    return 0;
}