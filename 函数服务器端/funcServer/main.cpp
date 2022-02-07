#include <cstdio>
#include <cstdlib>
#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Winsock2.h>
#include<WS2tcpip.h>

using namespace std;

#pragma comment(lib,"ws2_32.lib")


//void dllfunc()
//{
//	class dllManage dllm1;
//	string addDll = "C:\\Users\\ZY\\source\\repos\\第三阶段\\函数服务器端\\funcServer\\dlls\\math.dll";
//	string addDll2 = "C:\\Users\\ZY\\source\\repos\\第三阶段\\函数服务器端\\funcServer\\dlls\\strfunc.dll";
//	//新加的dll以后都通过命令行手动添加。
//	dllm1.writeLib(addDll);
//	dllm1.writeLib(addDll2);
//	dllm1.readLib(addDll2);
//	if (dllm1.GetFuncExplains()) {
//		const char** funcExArr = dllm1.GetFuncExplains();
//		const char** funcArr = dllm1.GetFuncNames();
//		for (int i = 0; i < dllm1.GetFuncNum(); i++)
//		{
//			cout << funcArr[i] << '\t' << funcExArr[i] << endl;
//		}
//	}
//}

const short HOST_PORT = 789;
const char* HOST_IP = "127.0.0.1";


int initWinSock()
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	if (0 == WSAStartup(wVersionRequested, &wsaData))
	{
		cout << "RUN-MSG:Sucess to connect to the lib of socket!" << endl;
		return 0;
	}
	else
	{
		cout << "ERR-MSG:Failed to connect to the lib of socket!" << endl;
		return -1;
	}
	//wVersionRequested是我们要求使用的WinSock的版本。
}

int main()
{
	int i;
	int maxConnectTimeout = 20;
	char revData[1024] = { '\0' };
	sockaddr_in sain;
	sockaddr_in remote_sain;
	int remote_len = sizeof(remote_sain);

	if (0 != initWinSock())
		return -1;


	SOCKET server_sock;
	server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_sock == INVALID_SOCKET)
	{
		printf("[-] SOCKET CREATE ERROR\n");
		return 0;
	}
	else {
		printf("[*] SOCKET CREATE SUCCESS\n");
	}

	sain.sin_family = AF_INET;
	sain.sin_port = htons(HOST_PORT);
	sain.sin_addr.S_un.S_addr = INADDR_ANY;

	if (bind(server_sock, (SOCKADDR*)& sain, sizeof(sain)) == SOCKET_ERROR)
	{
		printf("[-]CANNOT BIND\n");
		return 0;
	}
	else {
		printf("[*]BIND SUCCESS\n");
	}

	if (listen(server_sock, 20) == SOCKET_ERROR)
	{
		printf("[-]LISTENING ERROR\n");
		return 0;
	}
	else {
		printf("[*]LISTENING SUCCESS\n");
	}

	u_long mode = 1;
	ioctlsocket(server_sock, FIONBIO, &mode);//建立套接字所有权，这样信号可以被传递到合适的进程，设置非阻塞
	fd_set fd, newfd;
	static int s_num_socket = 0;
	FD_ZERO(&fd);
	FD_SET(server_sock, &fd);
	SOCKET sock_arr[64];
	sock_arr[s_num_socket++] = server_sock;
	newfd = fd;


	SOCKET client_sock;
	//while (1)
	//{
	//	printf("\n[*]Listening Remote-PC ...\n\n");
	//	client_sock = accept(server_sock, (SOCKADDR*)& remote_sain, &remote_len);//等待客户连接请求，此函数会阻塞
	//	if (client_sock == INVALID_SOCKET)
	//	{
	//		printf("[-]SOCKET RECV ERROR ...");
	//		continue;
	//	}
	//	char sendBuf[20] = { '\0' };
	//	printf("[+]PC from %s \n", inet_ntop(AF_INET, (void*)& remote_sain.sin_addr, sendBuf, 16));//输出客户端ip信息


	//	while (1)
	//	{
	//		memset(revData, 0, 1024);
	//		int ret = recv(client_sock, revData, 1023, 0);
	//		if (ret > 0)
	//		{
	//			revData[ret] = '\0';
	//			printf("%s\n", revData);
	//		}
	//	}
	//	closesocket(client_sock);
	//	break;
	//}

	while (1)
	{
		int ret = select(0, &newfd, NULL, NULL, NULL);//最后一个参数为NULL则无限等待信号出现
		if (ret < 0)
		{
			cout << "ERR-MSG:select error!" << endl;
			system("PAUSE");
			return -1;
		}
		//根据select返回的数目处理fd
		for (i = 0; i < s_num_socket; i++)
		{
			if (FD_ISSET(sock_arr[i], &newfd))
			{
				cout << "i is" << i << endl;
				if (sock_arr[i] == server_sock)//说明是服务器socket就绪，否则是client socket就绪
				{
					cout << "处理 server socket" << endl;
					if (client_sock = accept(server_sock, (SOCKADDR*)& remote_sain, &remote_len))
					{
						cout << "accept a new client" << endl;
						FD_SET(client_sock, &newfd);
						sock_arr[s_num_socket++] = client_sock;
						memset(revData, 0, 1024);
					}
				}
				else
				{
					cout << "处理 client socket" << endl;
					memset(revData, 0, 1024);
					int ret = recv(client_sock, revData, 1023, 0);
					if (ret > 0)
					{
						cout << revData << endl;
					}
					else {//坏连接
						if (WSAGetLastError() == WSAECONNRESET)
						{
							// Client socket closed
							cout << "Client socket " << sock_arr[i] << "has close" << endl;
							if (sock_arr[i] != INVALID_SOCKET)
							{
								FD_CLR(sock_arr[i], &newfd);
								closesocket(sock_arr[i]);
							}
							cout << "Remove client socket!" << sock_arr[i] << " success!" << endl;
							sock_arr[i--] = sock_arr[--s_num_socket];
							//如果是中间的client socket元素，这样删除不合理，改为容器
						}
					}
				}
			}
		}

		FD_ZERO(&newfd);
		//重新置newfd状态；
		for (i = 0; i < s_num_socket; i++)
			FD_SET(sock_arr[i], &newfd);
		//设置已有的socket
		Sleep(1);
	}



	printf("[-]Server OFF!");
	closesocket(server_sock);
	WSACleanup();
	return 0;
}
