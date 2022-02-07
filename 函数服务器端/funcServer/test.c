////#include<Windows.h>
////#include<stdio.h>
////
////_declspec(dllimport) int addFunc(int a, int b);
////
////typedef int (*func)(int, int);
////
////int main()
////{
////	func function;
////	double result;
////	HINSTANCE hinstLib = LoadLibrary("C:\\Users\\ZY\\source\\repos\\第三阶段\\dll2\\Dll1\\Debug\\Dll1.dll");
////	if (hinstLib == NULL) {
////		printf("ERROR:unable to load DLL\n");
////		return 1;
////	}
////
////	function = (func)GetProcAddress(hinstLib, "addFunc");
////	if (function == NULL)
////	{
////		printf("ERROR:unable to find DLL function\n");
////		FreeLibrary(hinstLib);
////		return 1;
////	}
////	result = function(1, 2);
////	FreeLibrary(hinstLib);
////	printf("Result = %f\n", result);
////	return 0;
////
////}
//#pragma once
//#include<iostream>
//#include<Windows.h>
//using namespace std;
//
//#pragma comment(lib,"ws2_32.lib")
//
//int initWinSock()
//{
//	WSADATA wsaData;
//	WORD wVersionRequested = MAKEWORD(1, 1);
//	if (0 == WSAStartup(wVersionRequested, &wsaData))
//	{
//		cout << "RUN-MSG:Sucess to connect to the lib of socket!" << endl;
//		return 0;
//	}
//	else
//	{
//		cout << "ERR-MSG:Failed to connect to the lib of socket!" << endl;
//		return -1;
//	}
//	//wVersionRequested是我们要求使用的WinSock的版本。
//}
//
//void* ServerThread(void* para)
//{
//}
//
//int main(int argc, char* argv[])
//{
//#ifdef _WIN32
//	cout << "win32!" << endl;
//	if (0 != initWinSock())
//		return -1;
//#else
//	cout << "Linux!" << endl;
//#endif
//	int sock, client_sock;
//	int sock_arr[64];
//	static int s_num_socket = 0;
//	short sport = 8989;
//	string sip = "127.0.0.1";
//
//	if (argc == 2)
//		sport = atoi(argv[1]);
//	if (argc == 3)
//	{
//		sport = atoi(argv[1]);
//		sip = argv[2];
//	}
//	cout << "ip is " << sip << ", port is " << sport << endl;
//
//	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	struct sockaddr_in sockaddr, client_addr;
//	fd_set fd, newfd;
//	int error_code;
//	int len, l;
//	string strread = "";
//
//	// sockaddr.sin_family      = AF_INET;
//   //  sockaddr.sin_port        = htons(sport);
//	// sockaddr.sin_addr.s_addr = inet_addr(INADDR_ANY);
//
//	sockaddr.sin_family = AF_INET;
//	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY); //inet_addr("127.0.0.1");//0;//;//inet_addr(INADDR_ANY);
//	sockaddr.sin_port = htons(sport);
//
//	if ((SOCKET_ERROR == bind(sock, (struct sockaddr*) & sockaddr, sizeof(struct sockaddr_in))))
//	{
//		cout << "ERR-MSG:bind error! :" << error_code << endl;
//		cout << WSAGetLastError() << endl;
//		system("PAUSE");
//		return -2;
//	}
//	else
//		cout << "RUN-MSG:bind sucess!" << endl;
//
//
//	if (SOCKET_ERROR == (error_code = listen(sock, 20)))
//	{
//		cout << "ERR-MSG:listen error! :" << error_code << endl;
//		system("PAUSE");
//		return -3;
//	}
//	else
//		cout << "RUN-MSG:listen success!the port is:" << sport << endl;
//
//	u_long mode = 1;
//	ioctlsocket(sock, FIONBIO, &mode);
//	// ioctlsocket(sock, FIONBIO,(u_long FAR*) &mode);//设置非阻塞的socket
//	char opt = 1;
//	// setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
//
//	FD_ZERO(&fd);
//	FD_SET(sock, &fd);
//	sock_arr[s_num_socket++] = sock;
//	char buffer[256];
//	memset(buffer, 0, 256);
//	len = sizeof(struct sockaddr_in);
//	int i;
//	newfd = fd;
//	while (1)
//	{
//
//		int ret = select(0, &newfd, NULL, NULL, NULL);
//		if (ret < 0)
//		{
//			cout << "ERR-MSG:select error!" << endl;
//			system("PAUSE");
//			return -1;
//		}
//		for (i = 0; i < s_num_socket; i++)
//		{
//
//			if (FD_ISSET(sock_arr[i], &newfd))
//			{
//				cout << "i is" << i << endl;
//				if (sock_arr[i] == sock)
//				{
//					cout << "RUN-MSG:it's me!" << endl;
//					if ((client_sock = accept(sock, (struct sockaddr*) & client_addr, &len)) != INVALID_SOCKET)
//					{
//						cout << "RUM-MSG:a new client!" << endl;
//						FD_SET(client_sock, &newfd);
//						sock_arr[s_num_socket++] = client_sock;
//						memset(buffer, 0, 256);
//					}
//				}
//				else
//				{
//					int ret = recv(sock_arr[i], buffer, 256, 0);
//					if (ret > 1)
//						cout << buffer << endl;
//					else
//					{
//						if (WSAGetLastError() == WSAECONNRESET)
//						{
//							// Client socket closed
//							cout << "RUN-MSG:Client socket " << sock_arr[i] << "has close" << endl;
//							if (sock_arr[i] != INVALID_SOCKET)
//							{
//								FD_CLR(sock_arr[i], &newfd);
//								close(sock_arr[i]);
//							}
//							cout << "RUN-MSG:Remove client socket!" << sock_arr[i] << " success!" << endl;
//							sock_arr[i--] = sock_arr[--s_num_socket];
//						} //坏连接
//					}//坏连接的情况
//				}
//			}
//
//		}
//		FD_ZERO(&newfd);
//		//重新置newfd状态；
//		for (i = 0; i < s_num_socket; i++)
//			FD_SET(sock_arr[i], &newfd);
//		sleep(1);
//
//
//		close_sock(sock);
//		WSACleanup();
//		system("PAUSE");
//		return EXIT_SUCCESS;
//	}
