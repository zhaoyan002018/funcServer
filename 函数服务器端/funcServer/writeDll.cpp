#include<Windows.h>
#include<winnt.h>
#include<iostream>
#include<fstream>
#include<string>
using namespace std;

const char* bodyStr = "Function server Libs Body v1.0";

//将dll文件写入lib中去，并将位置信息和文件名、函数名等以B+树的形式存储在索引文件中。
//需要用到C++流的知识
//1、先完成dll合并写到lib的操作，1.5、将索引信息顺序存储到文件中，并完成回溯，读取索引回复dll的操作。
//2、学习b+树的概念，写一个B+树的头文件（模板），
//3、编写dll文件信息对象，包括函数名、在lib中的位置信息等
//4、将以文件信息对象作为载体的B+树写到磁盘中去5、从磁盘中读取索引，并根据索引写临时dll，调用loadDll模块加载。
int main2()
{
	//cout << bodyStr.length() << endl;
	//以追加创建的方式打开FuncSLibsBody.lib文件，
	//将输入的dll写到lib文件中，编辑删除等修改已有数据的操作，放在索引结构之后
	string libPath = "C:\\Users\\ZY\\source\\repos\\第三阶段\\函数服务器端\\funcServer\\dlls\\FuncSLibsBody.lib";
	//没有lib时创建
	ifstream infile(libPath.c_str(), ios::in);
	if (!infile) {
		ofstream libfile(libPath.c_str(), ios::out | ios::binary);
		libfile.write(bodyStr, strlen(bodyStr));
		char ch[3];
		ch[0] = 0x0d;
		ch[1] = 0x0a;
		ch[2] = 0x1a;
		libfile.write(ch, 3);
		libfile.close();
	}
	infile.close();
	//接下来写dll
	ofstream libfile(libPath.c_str(), ios::out | ios::binary | ios::app);
	string dllPath = "C:\\Users\\ZY\\source\\repos\\第三阶段\\函数服务器端\\funcServer\\dlls\\math.dll";
	infile.open(dllPath.c_str(), ios::in | ios::binary);//文件默认是以ascii 码的方式打开的，以二进制打开则解决问题。
	/*
		文本文件读写；读时把\r\n替换成\n，写时把\n替换成\r\n;读到\x1a就设置EOF;读写的内容当字符看待
		二进制文件读写： 不对文件做任何处理，读到\x1a 当作一个普通的字节，继续读取
	*/
	if (!infile)
	{
		cout << "dll打开失败" << endl;
		return 1;
	}
	char c;
	infile.unsetf(ios::skipws);

	while (infile >> c)
	{
		libfile << c;
	}

	cout << infile.rdstate();
	infile.close();
	libfile.close();
	return 1;

}
