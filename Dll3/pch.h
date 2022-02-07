// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#include "framework.h"
#include<iostream>
using namespace std;
#endif //PCH_H


#ifdef __cplusplus    // If used by C++ code, 
extern "C" {          // we need to export the C interface
#endif

//必须要加extern "C"才能在主程序中找到addFunc，猜测应是不加声明时，会对函数名进行额外的修改。
//TODO用工具验证一下,dumpbin /EXPORTS my.dll 后发现，确实没有extern "C"时，函数名不是原本代码中的符号，而是增加了前后缀。但加上C声明则保持一致。
//这下就理解，因为在主程序测试时，采用了C的格式包含stdio.h去输出，而dll是c++格式编译的，则造成函数名不匹配。
//正确的做法是，dll和cpp都保持c++风格，如果不一致，可以用extern "c"将c++风格转为c。
//extern "C"编译时做了什么?是告诉c++编译采用c风格的编译方式

	const int funcN = 3;
	const char* myfunc[funcN] = { "mystrCmp","mystrRev","mystrTrc" };
	const char* myfuncEx[funcN] = { "bool mystrCmp(string&, string&)","void mystrRev(string&)","void mystrTrc(string&, int n)" };

	__declspec(dllexport) int getFuncN() { return funcN; }
	__declspec(dllexport) const char** getName() { return myfunc; }
	__declspec(dllexport) const char** getExplain() { return myfuncEx; }

	__declspec(dllexport) bool mystrCmp(string&, string&);
	__declspec(dllexport) void mystrRev(string&);
	__declspec(dllexport) void mystrTrc(string&, int n);


#ifdef __cplusplus
}
#endif
