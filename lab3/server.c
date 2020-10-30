#include <stdio.h>
#include <winsock2.h>
#include <string.h>

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
    printf("Winsock����...\n");

    //����������׽���
    SOCKET serverSock;
    int iSockErr = 0;
    
    // AF_INET����ַ�壬��ʾʹ��IP��ַ��
    // SOCK_STREAM���׽��֣�ʹ��tcp�ṩ�������ӵĿɿ��Ĵ��� 
    serverSock = socket(AF_INET, SOCK_STREAM, 0);   
 

    //�������˵�ַ�ṹ,ip��ַ���˿�
    SOCKADDR_IN serverAddr;
    int serverPort = 3333;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.S_un.S_addr = inet_addr("192.168.43.98");

    //���׽������������ַ��
    iSockErr = bind(serverSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    if (iSockErr==SOCKET_ERROR){
	    WSAGetLastError();//���ݲ�ͬ�Ĵ������ͽ��в�ͬ�Ĵ���
	    return -1;
    }


    //��������������
    iSockErr = listen(serverSock, 5);
    if(iSockErr!=0){
	    printf("Listen failed:%d\n",WSAGetLastError());
		exit(-4);
    }
    printf("��������: %d\n", ntohs(serverAddr.sin_port)); 
    printf("�ȴ��ͻ�������...\n");

    //��������
    SOCKET clientSock;     //�ͻ����׽���socket
    SOCKADDR_IN clientAddr;  //�ͻ��˵�ַ
    int clientAddrLen = sizeof(clientAddr);
    clientSock = accept(serverSock, (SOCKADDR*)&clientAddr, &clientAddrLen);

    //���ӳɹ�
    printf("���ӳɹ�!\n");
    printf("client ip is %s\n", inet_ntoa(clientAddr.sin_addr));
    printf("client port is %d\n", ntohs(clientAddr.sin_port));
    
    //���ͻ������Ϣ
    char sendBuff[256];
    char recvBuff[256];
    int dataLen = 0;
    while(1){
        recvBuff[0] = '\0';
        dataLen = recv(clientSock, recvBuff, 256, 0);
        recvBuff[dataLen] = '\0';
        if(strcmp(recvBuff, "quit") == 0)
            break;
        printf("������Ϣ: %s\n", recvBuff);
        
        //ת��Ϊ��д
        int i; 
        for (i = 0; i < strlen(recvBuff); ++i) {
            sendBuff[i] = toupper(recvBuff[i]);
        }
        sendBuff[i] = '\0';

        //���ؿͻ���
        printf("������Ϣ��%s\n",sendBuff);
        send(clientSock, sendBuff, strlen(sendBuff), 0);
        
        printf("\n");
    }

    //�ر�����
    shutdown(clientSock,2);
    shutdown(serverSock,2);
    closesocket(clientSock);
    closesocket(serverSock);
    WSACleanup();

    return 0;
}
