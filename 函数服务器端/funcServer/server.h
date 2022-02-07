#pragma once
#include"dllManage.h"
//下面处理服务器端代码

//1、管理函数调用，处理网络请求

/*
1、初始化socket包（WSAStartup）
2、创建套接字（socket）
3、绑定（bind）
4、监听（listen）
5、接受客户端连接（accept）
6、数据交互（recv/send）
7、断开连接（closesocket）
8、关闭套接字（closesocket）
*/
