#include <cstdio>
#include <cstdlib>
#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Winsock2.h>
#include<WS2tcpip.h>

using namespace std;

#pragma comment(lib,"ws2_32.lib")

const short HOST_PORT = 789;
const char* HOST_IP = "127.0.0.1";

int main(void)
{
	int Recvret = 0;
	char Sendbuff[255] = "\0";
	char Recvbuff[255] = "\0";
	WSADATA wsData;
	WORD sockVersion = MAKEWORD(2, 2);

	if (WSAStartup(sockVersion, &wsData) != 0)
	{
		printf("[-]SOCKET STARTUP ERROR\n");
		return 0;
	}
	else {
		printf("[*]SOCKET STARTUP SUCCESS\n");
	}

	SOCKET sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sClient == INVALID_SOCKET)
	{
		printf("[-]SOCKET CREATE ERROR\n");
		return 0;
	}
	else {
		printf("[*]SOCKET CREATE SUCCESS\n");
	}

	sockaddr_in Remote_sain;
	inet_pton(AF_INET, HOST_IP, (void*)& Remote_sain.sin_addr.S_un.S_addr);

	Remote_sain.sin_port = htons(HOST_PORT);
	Remote_sain.sin_family = AF_INET;
	if (connect(sClient, (SOCKADDR*)& Remote_sain, sizeof(Remote_sain)) == SOCKET_ERROR)//并不返回服务器端socket，而是继续使用本地socket
	{
		printf("[-]Can not connect to the Server\n");
		closesocket(sClient);
		return 0;
	}
	else {
		printf("[*]Listening to %s:%d ...OK\n", HOST_IP, HOST_PORT);
	}

	while (1)
	{
		/*if (Recvret = recv(sClient, Recvbuff, 255, 0) != 0)
		{
			Recvbuff[Recvret] = '\0';
			printf("[*]Server(%s):\n", HOST_IP);
		}*/
		cin >> Sendbuff;
		send(sClient, Sendbuff, 255, 0);
		printf("[*]Client:%s\n", Sendbuff);
	}
	closesocket(sClient);
	return 0;
}
