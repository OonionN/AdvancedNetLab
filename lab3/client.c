#include <stdio.h>
#include <winsock2.h>

int main(){
    //Winsock����
    //ʹ��WSAStartup()�������ϵͳЭ��ջ��װ���
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested=MAKEWORD(2,2);
    if(WSAStartup(wVersionRequested,&wsaData)!=0){  //��ʼ��ws2_32.dll��̬��
        printf("WSAStartup() failed!\n");//Winsock��ʼ������
        exit(-1);
    }
    if(wsaData.wVersion!=wVersionRequested){
        printf("The version of Winsock is not suited!\n");//Winsock�汾��ƥ��
        WSACleanup();//������ws2_32.dll�ĵ���
        exit(-1);
    }
    //˵��ws2_32.dll��ȷ����
    printf("Load ws2_32.dll successfully!\n");

    //��ʼ��Ҫ���ӵķ����socket�͵�ַ
    SOCKET serverSock;
    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN serverAddr;
    int serverPort = 3333;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    //���ӷ�����
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
    printf("connect success\n");

    //���ͻ������Ϣ
    char sendBuff[256];
    char recvBuff[256];
    while(1){
        memset(sendBuff, 0, sizeof(sendBuff));
        printf("����Ҫ���͵���Ϣ: ");
        scanf("%s", sendBuff);
        send(serverSock, sendBuff, sizeof(sendBuff), 0);
        if(strcmp(sendBuff, "quit") == 0)
            break;
        memset(recvBuff, 0, sizeof(recvBuff));
        recv(serverSock, recvBuff, sizeof(recvBuff), 0);
        printf("���յ���Ϣ: %s\n", recvBuff);
    }
    
    //�ر�����
    shutdown(serverSock,2);
    closesocket(serverSock);
    WSACleanup();

    return 0;
}
