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

    //����������׽���
    SOCKET serverSock;
    serverSock = socket(AF_INET, SOCK_STREAM, 0);

    //�������˵�ַ�ṹ,ip��ַ���˿�
    SOCKADDR_IN serverAddr;
    int serverPort = 3333;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    //���׽������������ַ��
    bind(serverSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));

    //��������������
    listen(serverSock, 5);

    //��������
    SOCKET clientSock;     //�ͻ����׽���socket
    SOCKADDR_IN clientAddr;  //�ͻ��˵�ַ
    int clientAddrLen = sizeof(clientAddr);
    clientSock = accept(serverSock, (SOCKADDR*)&clientAddr, &clientAddrLen);

    //���ӳɹ�
    printf("connect success\n");

    //���ͻ������Ϣ
    char sendBuff[256];
    char recvBuff[256];
    while(1){
        memset(recvBuff, 0, sizeof(recvBuff));
        recv(clientSock, recvBuff, sizeof(recvBuff), 0);
        printf("���յ���Ϣ: %s\n", recvBuff);
        if(strcmp(recvBuff, "quit") == 0)
            break;

        //ת��Ϊ��д
        memset(sendBuff, 0, sizeof(sendBuff));
        int i; 
        for (i = 0; i < strlen(recvBuff); ++i) {
            sendBuff[i] = toupper(recvBuff[i]);
        }

        //���ؿͻ���
        send(clientSock, sendBuff, sizeof(sendBuff), 0);

    }

    //�ر�����
    shutdown(clientSock,2);
    shutdown(serverSock,2);
    closesocket(clientSock);
    closesocket(serverSock);
    WSACleanup();

    return 0;
}
