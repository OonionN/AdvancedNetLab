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

    //初始化要连接的服务端socket和地址
    SOCKET serverSock;
    serverSock = socket(AF_INET, SOCK_STREAM, 0); 
    SOCKADDR_IN serverAddr;
    int serverPort = 3333;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    //连接服务器
    int iSockErr = 0;
    iSockErr = connect(serverSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    if (iSockErr == SOCKET_ERROR)
    {
        printf("connect unsuccessful!\n");
        iSockErr = closesocket(serverSock);
        if (iSockErr == SOCKET_ERROR)
            printf("close socket unsuccessful!\n");
        return -1;
    }
    printf("连接成功!\n");
    printf("server ip is %s\n", inet_ntoa(serverAddr.sin_addr));
    printf("server port is %d\n", ntohs(serverAddr.sin_port));

    //发送或接收消息
    char sendBuff[256];
    char recvBuff[256];
    int dataLen = 0;
    while(1){
    	sendBuff[0] = '\0';
        printf("输入要发送的消息: ");
        gets(sendBuff);
        send(serverSock, sendBuff, strlen(sendBuff), 0);
        if(strcmp(sendBuff, "quit") == 0)
            break;
            
        recvBuff[0] = '\0';
        dataLen = recv(serverSock, recvBuff, 256, 0);
        recvBuff[dataLen] = '\0';
        printf("接收消息: %s\n", recvBuff);
        printf("\n");
    }
    
    //关闭连接
    shutdown(serverSock,2);
    closesocket(serverSock);
    WSACleanup();

    return 0;
}
