# computer-network-hw
2025春计算机网络实验
## 实验环境：linux 语言：c++
## hw1：基础echo&&ftp
### echo 
服务器和客户端简单通信
#### 主要函数
##### 服务器方面初始时需要执行的操作：
int socket ()		建立一个Socket  
int bind()			与某个端口绑定  
int listen()		开始监听端口  
int accept()		等待/接受客户端的连接请求  
##### 客户端需要执行的操作：
int socket ()		建立一个Socket  
int connect() 		连接到服务器  
#### 伯克利套接字
（1）int socket (int domain, int type, int protocol)
功能：创建套接字。  
返回值：返回值是新创建套接字的句柄，即以后引用该套接字时使用的标识符，错误时返回-1。  
参数 domain 描述将使用的协议族。  
AF_INET：用于表示因特网协议族。  
AF_UNIX：用于表示U n i x 管道功能。  
参数 type表明通信的语义。  
	SOCK_STREAM：字节流服务，即流式套接字，FTP使用流式套接字。   
	SOCK_DGRAM：面向消息的服务，即数据报套接字。  
参数 protocol 则指明将要用到的特定协议。  
	IPPROTO_TCP：指的是使用TCP协议。  
（2）int bind(int socket, struct socketaddr *address, int addr_len)  
功能：将创建的 socket 与 adress （包含 IP和port信息）绑定。  
返回值：在错误的时候会返回-1。  
参数 socket 描述将使用的套接字。  
参数 addr_len 描述的是参数adress的长度。  
参数 adress 描述将绑定的地址。  
参数中用到的数据结构：  
	struct socketaddr：在因特网协议中地址描述使用的数据结构。  
	struct sockaddr {   
       unsigned short sa_family;   
       char sa_data[14];   
};   
sa_family 描述将使用的协议族，一般为AF_INET 。  
sa_data 为套接口储存目标地址和端口信息。  
注意：有两种字节排列顺序：重要的字节在前面，或者不重要的字节在前面。前一种叫“网络字节顺序 (Network Byte Order)”。不同机器，不同语言之间的字节存放顺序是不一样的，所以在网络上传输数据时，一定要转成网络字节顺序。 数据必须按照 NBO 顺序，那么你要调用函数(例如 htons() )来将他从“本机字节顺序 (Host Byte Order) ”转换过来。  
有以下四个主要的转换函数：  
	htons()	将Short型数据转换为网络字节类型。  
	htonl() 	将Long型数据转换为网络字节类型。  
	ntohs() 	将Short型数据转换为本地字节类型。  
	ntohl() 	将Long型数据转换为本地字节类型。  
事实上，上面的struct sockaddr并不好用，因它没有明确细化内部结构。于是程序员创造了一个并列的结构sockaddr_in ，它可以与结构sockaddr互相转换。  
	struct sockaddr_in {   
              short int sin_family;    
              unsigned short int sin_port;   
              struct in_addr sin_addr;   
              unsigned char sin_zero[8];   
}  
首先，你需要将 IP 地址储存到 struct sockaddr_in ina中。  
当IP地址的形式是“numbers-and-dots”时(如“32.41.5.10”)，你要用的函数是 inet_addr()：  
	ina.sin_addr.s_addr = inet_addr(“132.241.5.10”);  
注意：inet_addr() 返回的地址已经是按照网络字节顺序的，你没有必要再去调用 htonl()。  
当IP地址的形式已经是long时，则你需要使用htonl()函数来进行转换。  
（3）int listen(int socket, int backlog)  
功能：定义在指定的 Socket 上可有多少个待处理的连接。  
返回值：在发生错误时返回-1。  
参数socket 是调用 socket() 返回的套接口文件描述符。  
参数backlog 是在进入队列中允许的连接数目。  
（4）int accept(int socket, struct socketaddr *address, int addr_len)  
功能：接收客户端连接请求。  
返回值：如果连接成功， 函数将返回一个新的套接口文件描述符。接下来，就可以对这个描述符进行发送 (send()) 和接收 (recv()) 操作了，错误时返回-1。  
参数socket 套接口文件描述符。  
参数address struct sockaddr_in 的指针。  
参数addrlen 长度，常为 sizeof(struct sockaddr_in)。  
（5）int connect(int socket, struct sockaddr *serv_addr, int addrlen)   
功能：在客户端被用于连接到服务器。  
返回值：发生错误的时候返回-1  
参数socket 套接口文件描述符。  
参数serv_addr 包含是服务器的地址和端口信息。  
参数addrlen 长度，常为 sizeof(struct sockaddr_in)。  
（6）int setsockopt(int socket,int level,int optname,const void *optval,socklen_t *optlen)   
功能：设置套接字行为 。  
参数level指定控制套接字的层次。  
SOL_SOCKET表示通用套接字选项。  
参数optname	指定控制的方式(选项的名称)。  
		SO_REUSERADDR表示重用本地地址和端口。  
参数optval 指示相应的行为，如功能的开启或关闭。  

