#include <stdio.h>
#include <winsock2.h>
#include <string.h>

int main(){

    //Winsock启动
    //使用WSAStartup()函数检查系统协议栈安装情况
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested=MAKEWORD(2,2);
    if(WSAStartup(wVersionRequested,&wsaData)!=0){  //初始化ws2_32.dll动态库
        printf("WSAStartup() failed!\n");//Winsock初始化错误
        exit(-1);
    }
    if(wsaData.wVersion!=wVersionRequested){
        printf("The version of Winsock is not suited!\n");//Winsock版本不匹配
        WSACleanup();//结束对ws2_32.dll的调用
        exit(-1);
    }
    //说明ws2_32.dll正确加载
    printf("Winsock启动...\n");

    //创建服务端套接字
    SOCKET serverSock;
    int iSockErr = 0;
    
    // AF_INET，地址族，表示使用IP地址族
    // SOCK_STREAM流套接字，使用tcp提供的有连接的可靠的传输 
    serverSock = socket(AF_INET, SOCK_STREAM, 0);   
 

    //定义服务端地址结构,ip地址、端口
    SOCKADDR_IN serverAddr;
    int serverPort = 3333;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.S_un.S_addr = inet_addr("192.168.43.98");

    //将套接字与服务器地址绑定
    iSockErr = bind(serverSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    if (iSockErr==SOCKET_ERROR){
	    WSAGetLastError();//根据不同的错误类型进行不同的处理
	    return -1;
    }


    //服务器启动监听
    iSockErr = listen(serverSock, 5);
    if(iSockErr!=0){
	    printf("Listen failed:%d\n",WSAGetLastError());
		exit(-4);
    }
    printf("启动监听: %d\n", ntohs(serverAddr.sin_port)); 
    printf("等待客户端连接...\n");

    //接收请求
    SOCKET clientSock;     //客户端套接字socket
    SOCKADDR_IN clientAddr;  //客户端地址
    int clientAddrLen = sizeof(clientAddr);
    clientSock = accept(serverSock, (SOCKADDR*)&clientAddr, &clientAddrLen);

    //连接成功
    printf("连接成功!\n");
    printf("client ip is %s\n", inet_ntoa(clientAddr.sin_addr));
    printf("client port is %d\n", ntohs(clientAddr.sin_port));
    
    //发送或接收消息
    char sendBuff[256];
    char recvBuff[256];
    int dataLen = 0;
    while(1){
        recvBuff[0] = '\0';
        dataLen = recv(clientSock, recvBuff, 256, 0);
        recvBuff[dataLen] = '\0';
        if(strcmp(recvBuff, "quit") == 0)
            break;
        printf("接收消息: %s\n", recvBuff);
        
        //转换为大写
        int i; 
        for (i = 0; i < strlen(recvBuff); ++i) {
            sendBuff[i] = toupper(recvBuff[i]);
        }
        sendBuff[i] = '\0';

        //返回客户端
        printf("发送消息：%s\n",sendBuff);
        send(clientSock, sendBuff, strlen(sendBuff), 0);
        
        printf("\n");
    }

    //关闭连接
    shutdown(clientSock,2);
    shutdown(serverSock,2);
    closesocket(clientSock);
    closesocket(serverSock);
    WSACleanup();

    return 0;
}
