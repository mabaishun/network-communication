/*****************************************************************
*   Copyright (C) 2020 Sangfor Ltd. All rights reserved.
*   
*   File Name    ：uban_tcp_base.cpp
*   Author       ：spider
*   Date Time    ：2020年04月22日
*   Mail         ：563621005@qq.com
*   Describe     ：
*
*****************************************************************/

#include "mbs_tcp_base.h"


TcpBase::TcpBase():listenflag(false),listenfd(-1),addrlen(-1)
{
    bzero(&cliaddr,sizeof(struct sockaddr_in));
    //忽略一些信号 SIGHUP, SIGPIPE
    //SIGPIPE:如果客户端关闭，服务端再次write就会产生
    //SIGHUP:如果terminal关闭，会给当前进程发送该信号
    if (signal(SIGHUP, SIG_IGN) == SIG_ERR) {
        fprintf(stderr, "signal ignore SIGHUP\n");
    }
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        fprintf(stderr, "signal ignore SIGPIPE\n");
    }

}
TcpBase::~TcpBase()
{
    Close();
}


/// @brief: 作者：spider
/// @brief: 时间：2020.1.1
/// @brief: 邮箱：563621005@qq.com
/// @brief: 环境：windows下VS2013 或者 Linux
/// @brief:	函数名称：listen
/// @brief:	函数作用： 创建并监听套接字
/// @brief: 函数参数: 1. host	ip地址，如果为空，则地址为ADDR_ANY
/// @brief:			  2. serv	要监听的端口(字符串格式)
/// @brief: 函数返回值：0-成功，-1 失败
int TcpBase::listen(const char *host, uint16_t port)
{
    if(listenflag)
    {
        return listenfd;
    }
	int n;
	const int on = 1;

	//定义addrinfo结构
	addrinfo hints;	//hints用于调用者在这个结构中填入关于期望返回的信息类型的暗示
	addrinfo *res, *ressave;//res 返回一个指向addrinfo结构体链表的指针,ressave保存res的结果
	//初始化hinds为空
	memset(&hints, 0, sizeof(addrinfo));

	//通常服务器端在调用getaddrinfo之前，ai_flags 设置AI_PASSIVE，用于 bind；
	hints.ai_flags = AI_PASSIVE;
	//ai_family 只是有地址为v4地址或v6地址的区别
	hints.ai_family = AF_INET;
	//ailistenfdtype 协议类型 比如：tcp、udp等
	hints.ai_socktype = SOCK_STREAM;



    char serv[8];
    snprintf(serv,8,"%d",port);

	//getaddrinfo() 与协议无关，既可以用于IPv4，也可以用于IPv6
	//作用：处理名字到地址以及服务到端口的转换
	//函数原型：int getaddrinfo( const char *hostname, const char *service, const struct addrinfo *hints, struct addrinfo **result );
	//参数：hostname：一个主机名或者地址串
	//		service：服务名可以是十进制的端口号，也可以是已定义的服务名称，如ftp,http等
	//		hints:可以是一个空指针，也可以是一个指向某个addrinfo结构体的指针，调用者在这个结构中填入关于期望返回的信息类型的暗示
	//		result:本函数通过result指针参数返回一个指向addrinfo结构体链表的指针。
	//返回值：0-成功，非0-出错
	if ((n = getaddrinfo(host, serv, &hints, &res)) != 0)
	{
        std::cout << "tcp_listen error for " << host << "," << serv
			<< ":" << gai_strerror(n) << std::endl;
		return -1;
	}

	//保存指针，因为res在addrinfo函数簇内部中是动态分配的，下面的操作中有可能有改动，不利于freeaddrinfo();所以交给另一个指针保存
	ressave = res;

	//addrinfo是一个链结构，所以用循环遍历,getaddrinfo()不失败的情况下至少有一个节点，所以用do ...while()
	do {
		///创建socket套接字
		listenfd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		//如果这个节点上的数据创建套接字失败就继续下一个节点
		if (listenfd < 0)
			continue;
        std::cout << "创建套接字成功" << std::endl;

		//socket 套接字描述符
		//level 被设置的选项的级别，如果想要在套接字级别上设置选项，就必须把level设置为SOL_SOCKET
		//option_name 指定准备设置的选项，
		//option_name可以有哪些取值，这取决于level，以linux 2.6内核为例（在不同的平台上，这种关系可能会有不同），在套接字级别上(SOL_SOCKET)
		//optlen 则为整形数或结构的长度。
		setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&on, sizeof(on));
		
		//套接字地址绑定，如果绑定成功就结束循环
		if ( ::bind(listenfd, res->ai_addr, res->ai_addrlen) == 0)
        {
			break;
        }
        else
        {
            std::cout << PRINTINFO << "绑定失败" << std::endl;
        }
		//否则关闭已经打开的套接字
		close(listenfd);
	} while ((res = res->ai_next) != NULL);

	//最后操作的socket() 或者 bind() 发生错误
	if (res == NULL)
	{
        std::cout << "Tcpserver::listen() error for " << host << ":" << serv << std::endl;
		return -1;
	}
	//在绑定成功的套接字上监听，最大未决连接数为BACKLOG（如果没有改动的话，应该是50）
	if (::listen(listenfd, 5) == -1)
	{
		close(listenfd);
        std::cout << "TcpServer::listen() ::listen() error: " << errno << std::endl;
		return -1;
	}

    listenflag = true;
	std::cout << "监听套接字成功" << std::endl;
	if (addrlen == 0)
		addrlen = res->ai_addrlen;
	//释放addrinfo结构，getaddrinfo返回的所有存储空间都是动态获取的
	freeaddrinfo(ressave);
	//成功返回0
	return 0;
}


int TcpBase::accept()
{
	//定义客户端地址结构
	//sockaddr_in cliaddr;
	//初始化客户端地址结构
	//memset(&cliaddr, 0, sizeof(sockaddr_in));
	//获取客户端地址结构长度
	socklen_t len = sizeof(cliaddr);
	
    std::cout << "等待客户端连接到来..." << std::endl;
	//等待连接到来
    while(true)
    {
        int accsock = ::accept(listenfd, (sockaddr*)&cliaddr, &len);
        if (accsock == -1)
        {
            if(accsock == EINTR)
            {
                std::cout << PRINTINFO  << "accept 链接错误:" << strerror(errno) << std::endl;
                continue;
            }
            else if(errno == EMFILE)
            {
                std::cout << PRINTINFO << "accept 链接错误:" << strerror(errno) << std::endl; 
            }
            else if(errno == EAGAIN)
            {
                std::cout << PRINTINFO << "accpet 链接错误:" << strerror(errno) << std::endl;
                break;
            }
            else
            {
                std::cout << PRINTINFO << "accept 链接错误" << std::endl;
                return -1;
            }
        }
        else
        {
            char dest[128];
            memset(dest, 0, 128);
            inet_ntop(AF_INET, &cliaddr.sin_addr, dest, 16);
            std::cout << "新客户端加入 sockfd = [" << accsock << "] Ip = [" << dest << "]" << std::endl;
            return accsock;
        }
    }
    std::cout << PRINTINFO << "accept 链接错误" << std::endl;
    return -1;
}

void TcpBase::Close()
{
    if(listenfd != -1)
    {
        ::close(listenfd);
    }
}
