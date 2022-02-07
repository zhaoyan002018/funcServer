// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"

// 当使用预编译的头时，需要使用此源文件，编译才能成功。
bool mystrCmp(string& s1, string& s2)
{
	return s1 < s2;
}
void mystrRev(string& s)
{
	s.reserve();
}
void mystrTrc(string& s, int n)
{
	s.substr(n);
}
