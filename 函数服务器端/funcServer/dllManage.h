#pragma once
#include<Windows.h>
#include<winnt.h>
#include<iostream>
#include<fstream>
#include<string>
using namespace std;

//dllManage.h文件的目的：是为了编写对dll文件的加载以及写入的操作，并封装到对象中去。
//首先对dll文件的操作分为俩个方向
//1、写入，将输入的dll文件，写入索引文件以及函数体lib文件中。
//2、加载，根据索引文件，从函数体lib中找到相应的dll文件部分放到一个临时文件中加载进内存，并调用相应函数。
//那么是一个对象？还是俩个对象？数据部分是什么，方法又是什么？
//对象是一个，即dll文件
//写入操作需要的数据有：一个.so文件即可，但对这个.so有额外的要求，这些要求的数据格式可以体现在对象的数据成员中，比如：
//name 函数名数组，funcn 函数个数，funcex,函数声明解释（用于告知客服端使用方式）,另外还有函数指针的格式封装，目前是预先指定，之后考虑用嵌入汇编代码实现

//load操作需要的数据有：保存的索引lib和函数体lib，加载到内存中后去调用函数的操作，放到服务器对象中去做。

//本对象只包含对dll的加载和写入操作，如何使用这部分被加载到内存的dll，交给服务器对象来做。

typedef double (*func)(double, double);//如果类型和函数不一致会发生什么？虽然入口函数地址是对的，但是参数长度不同，函数体处理时就会有处理字长错误的问题
//根据客户端传来的参数结构体获得参数的个数，继而通过嵌入汇编代码实现无差别调用，这部分放在客户端完成参数传递后进行，目前事先先存储号函数类型对象。


typedef const char** (*funcGetF)();
typedef const int (*funcGetN)();

//目前采用顺序结构存储，之后改为B+树存储，因为顺序查找不如B+树按dll名称排好序进行折半查找有效率。
typedef struct {
	char dllName[255];
	long begin;
	long end;
}indexInfo;


class dllManage
{
	funcGetF myFunc;	//函数名
	funcGetN myFuncN;	//函数个数
	funcGetF myFuncEx;	//函数声明

	HINSTANCE hinstLib;	//dll句柄
	string libBodyPath;
	string libIndexPath;
	const char* libBodyStr = "Function server Libs Body v1.0";
	const char* libIndexStr = "Function server Libs Index v1.0";
	indexInfo idxInfo;
	void copyByChar(istream* src, ostream* dst, int n);
public:
	dllManage(
		string libBodyPath = "C:\\Users\\ZY\\source\\repos\\第三阶段\\函数服务器端\\funcServer\\dlls\\FuncSLibsBody.lib",
		string libIndexPath = "C:\\Users\\ZY\\source\\repos\\第三阶段\\函数服务器端\\funcServer\\dlls\\FuncSLibsIndex.idx"
	);
	~dllManage();
	const char** GetFuncNames();
	const char** GetFuncExplains();
	int GetFuncNum();
	bool loadDll(string dllPath = "C:\\Users\\ZY\\source\\repos\\第三阶段\\函数服务器端\\funcServer\\dlls\\tmp.dll");//将dll加载到内存中。
	bool readLib(string dllPath);//从lib中读取dll，输入包名+函数名,用.分隔，得到dll在lib中的起始终止位置，并写到临时dll中，再调用loadDll加载进内存。
	bool writeLib(string dllPath);//将dll写入lib中
	void clearDll();
};

dllManage::dllManage(string libBodyPath, string libIndexPath)
{
	this->libBodyPath = libBodyPath;
	this->libIndexPath = libIndexPath;
	string libPath[2] = { libBodyPath ,libIndexPath };
	const char* libStr[2] = { libBodyStr ,libIndexStr };
	for (int i = 0; i < 2; i++)
	{
		ifstream infile(libPath[i].c_str(), ios::in);
		if (!infile) {
			ofstream bodyFile(libPath[i], ios::out | ios::binary);
			bodyFile.write(libStr[i], strlen(libStr[i]));
			char ch[3];
			ch[0] = 0x0d;
			ch[1] = 0x0a;
			ch[2] = 0x1a;
			bodyFile.write(ch, 3);
			bodyFile.close();
		}
		infile.close();
	}

}
dllManage::~dllManage()
{
	if (hinstLib)
	{
		FreeLibrary(hinstLib);
		hinstLib = NULL;
		myFunc = NULL;
		myFuncN = NULL;
		myFuncEx = NULL;
	}
}
void dllManage::clearDll()
{
	if (hinstLib)
	{
		FreeLibrary(hinstLib);
		hinstLib = NULL;
		myFunc = NULL;
		myFuncN = NULL;
		myFuncEx = NULL;
	}
}
bool dllManage::loadDll(string dllPath)
{
	clearDll();//清除上一个被加载的dll。如果想要实现多个dll存在内存中，就要对应申请多少个dllManage对象。
	//注意如果同时加载多个dll时，临时文件的命名问题。
	hinstLib = LoadLibrary(dllPath.c_str());
	if (hinstLib == NULL) {
		cout << ("ERROR:unable to load DLL\n") << endl;
		return false;
	}
	myFunc = (funcGetF)GetProcAddress(hinstLib, "getName");
	myFuncN = (funcGetN)GetProcAddress(hinstLib, "getFuncN");
	myFuncEx = (funcGetF)GetProcAddress(hinstLib, "getExplain");
	return true;
}
void dllManage::copyByChar(istream* src, ostream* dst, int n = NULL)
{
	char c;
	(*src).unsetf(ios::skipws);
	if (!n)
		while (*src >> c)
		{
			*dst << c;
		}
	else
		while (n)
		{
			*src >> c;
			if ((*src).rdstate())
			{
				cout << "error函数体写入临时文件提前结束,可能造成dll文件异常" << endl;
				break;
			}
			*dst << c;
			n--;
		}
}
bool dllManage::writeLib(string dllPath)
{
	//1、写libbody部分。
	ofstream bodyFile(libBodyPath, ios::out | ios::binary | ios::app);
	if (!bodyFile)
	{
		cout << "不存在FuncSLibsBody路径下的目录" << endl;
		return false;
	}

	ifstream infile(dllPath.c_str(), ios::in | ios::binary);//文件默认是以ascii 码的方式打开的，以二进制打开则解决问题。
	if (!infile)
	{
		cout << "dll打开失败" << endl;
		return false;
	}
	/*
	文本文件读写；读时把\r\n替换成\n，写时把\n替换成\r\n;读到\x1a就设置EOF;读写的内容当字符看待
	二进制文件读写： 不对文件做任何处理，读到\x1a 当作一个普通的字节，继续读取
	*/
	//得到当前起始位置,发现尽管open设置了app，指针还是在begin位置而不是end，现在手动操作指针到end位置。TODOios::app的机制
	bodyFile.seekp(0, ios::end);
	idxInfo.begin = bodyFile.tellp();

	copyByChar(&infile, &bodyFile);

	idxInfo.end = bodyFile.tellp();

	infile.close();//在关闭前还要清除。
	bodyFile.close();

	/*
	思考写index部分
	包含包的名称（即dll名称）、函数名称、包在lib函数库中的起始位置
	函数名称需要放到bodyLib中吗？找到包名，就能找到起始终止位置的话，加载到内存里，就能读取出函数名了。
	如果客户端方向传来了包名.函数名，我们取包名来找到dll位置，加载到内存后，再根据函数名去调用函数，如果函数名出错，只能在载入dll后才能解决
	如果放在索引文件中，此时可以在载入前做了判断，但索引文件的设计就会因此变得更复杂
	可以一开始扫描一遍所有的lib中的dll文件，把其中的所有函数名生成一个hash，对映到相应的dll文件，此时虽然索引没有存函数名，也可以在载入前检验。

	也就是说正常读取函数的流程是这样的，客户端发送包名.函数名+参数进来，首先根据函数名找到对应的dll文件，如果找不到说明不存在，
	如果找到dll文件名，再去读取索引文件，根据文件名找到在bodylib中的起始终止位置，并写到一个临时文件中，然后加载临时的dll，再根据函数名去调用相应的函数去。
	并将返回值返回给客户端完成一次正常调用流程。

	这其中需要注意的有，包机制的体现，可想而知不同文件中可以有相同的函数，其实dll文件本身就实现了包机制，因此可以将文件名作为包名即可。在初始化扫描时，
	生成相应函数名-文件名的hash表时，就把文件名作为包名即可，检查是否在该包中，即可通过检查hash表实现。
	*/

	//2、写libindex部分。
	ofstream indexFile(libIndexPath, ios::out | ios::binary | ios::app);//顺序添加
	if (!indexFile)
	{
		cout << "不存在index路径下的目录" << endl;
		return false;
	}
	//通过dllpath的路径得到dll文件名。
	int pos = dllPath.find_last_of('\\');
	strcpy_s(idxInfo.dllName, dllPath.substr(pos + 1).c_str());

	indexFile.write(idxInfo.dllName, 255);
	indexFile.write((char*)& idxInfo.begin, sizeof(long));
	indexFile.write((char*)& idxInfo.end, sizeof(long));

	cout << "写入" << idxInfo.dllName << '\t' << idxInfo.begin << '\t' << idxInfo.end << endl;

	//TODO给idxInfo清空
	indexFile.close();

	return true;
}
bool dllManage::readLib(string dllPath)
{
	//1.1读取索引
	ifstream indexFile(libIndexPath, ios::in | ios::binary);
	if (!indexFile)
	{
		cout << "索引文件打开失败" << endl;
		return false;
	}
	indexFile.seekg(strlen(libIndexStr) + 3, ios::beg);//定位到真正dll文件数据部分
	int pos = dllPath.find_last_of('\\');
	string dllName = dllPath.substr(pos + 1);
	while (1)
	{

		indexFile.read(idxInfo.dllName, 255);
		indexFile.read((char*)& idxInfo.begin, sizeof(long));
		indexFile.read((char*)& idxInfo.end, sizeof(long));
		if (!dllName.compare(idxInfo.dllName))
		{
			break;
		}
		if (indexFile.eof())
		{
			cout << "未找到相匹配的dll" << endl;
			return false;
		}
	}

	cout << "读取" << idxInfo.dllName << '\t' << idxInfo.begin << '\t' << idxInfo.end << endl;

	//1.2、打开读取bodylib文件
	ifstream bodyFile(libBodyPath, ios::in | ios::binary);
	bodyFile.seekg(idxInfo.begin, ios::beg);

	//2.1写到临时文件中去：
	fstream tmpDll("C:\\Users\\ZY\\source\\repos\\第三阶段\\函数服务器端\\funcServer\\dlls\\tmp.dll", ios::in | ios::binary);//如何在当前文件夹下创建呢？
	if (tmpDll)
	{
		cout << "存在重名的dll临时文件" << endl;
		tmpDll.close();
		return false;
		//TODO如果存在该名称，则更改名称
	}

	tmpDll.open("C:\\Users\\ZY\\source\\repos\\第三阶段\\函数服务器端\\funcServer\\dlls\\tmp.dll", ios::out | ios::binary);


	long dlllen = idxInfo.end - idxInfo.begin;
	copyByChar(&bodyFile, &tmpDll, dlllen);

	bodyFile.close();
	tmpDll.close();

	//2.2load临时文件。
	loadDll();

	return true;

}//从lib中读取dll，输入包名+函数名,用.分隔，得到dll在lib中的起始终止位置，并写到临时dll中，再调用loadDll加载进内存。


const char** dllManage::GetFuncNames()
{
	if (myFuncN)
		return myFunc();
	return NULL;
}
const char** dllManage::GetFuncExplains()
{
	if (myFuncEx)
		return myFuncEx();
	return NULL;
}
int dllManage::GetFuncNum()
{
	if (myFuncN)
		return myFuncN();
	return NULL;
}
