#include <stdio.h>
#include <winsock2.h>

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
    printf("Load ws2_32.dll successfully!\n");

    //创建服务端套接字
    SOCKET serverSock;
    serverSock = socket(AF_INET, SOCK_STREAM, 0);

    //定义服务端地址结构,ip地址、端口
    SOCKADDR_IN serverAddr;
    int serverPort = 3333;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    //将套接字与服务器地址绑定
    bind(serverSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));

    //服务器启动监听
    listen(serverSock, 5);

    //接收请求
    SOCKET clientSock;     //客户端套接字socket
    SOCKADDR_IN clientAddr;  //客户端地址
    int clientAddrLen = sizeof(clientAddr);
    clientSock = accept(serverSock, (SOCKADDR*)&clientAddr, &clientAddrLen);

    //连接成功
    printf("connect success\n");

    //发送或接收消息
    char sendBuff[256];
    char recvBuff[256];
    while(1){
        memset(recvBuff, 0, sizeof(recvBuff));
        recv(clientSock, recvBuff, sizeof(recvBuff), 0);
        printf("接收的消息: %s\n", recvBuff);
        if(strcmp(recvBuff, "quit") == 0)
            break;

        //转换为大写
        memset(sendBuff, 0, sizeof(sendBuff));
        int i; 
        for (i = 0; i < strlen(recvBuff); ++i) {
            sendBuff[i] = toupper(recvBuff[i]);
        }

        //返回客户端
        send(clientSock, sendBuff, sizeof(sendBuff), 0);

    }

    //关闭连接
    shutdown(clientSock,2);
    shutdown(serverSock,2);
    closesocket(clientSock);
    closesocket(serverSock);
    WSACleanup();

    return 0;
}
