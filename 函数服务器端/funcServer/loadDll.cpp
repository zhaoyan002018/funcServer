#include<Windows.h>
#include<winnt.h>
#include<iostream>
//
//#include<DbgHelp.h>
//#include<ImageHlp.h>
//#include <tchar.h>
using namespace std;

//_declspec(dllimport) double addFunc(double a, double b);
//_declspec(dllimport) const char** getName();
//_declspec(dllimport) int getFuncN();

typedef const char** (*funcGetF)();
typedef const int (*funcGetN)();

typedef double (*func)(double, double);//如果类型和函数不一致会发生什么？虽然入口函数地址是对的，但是参数长度不同，函数体处理时就会有处理字长错误的问题
//根据客户端传来的参数结构体获得参数的个数，继而通过嵌入汇编代码实现无差别调用，这部分放在客户端完成参数传递后进行，目前事先先存储号函数类型对象。


//还需要根据dll句柄，获得dll导出的函数名

//typedef PVOID(CALLBACK* PFNEXPORTFUNC)(PIMAGE_NT_HEADERS, PVOID, ULONG, PIMAGE_SECTION_HEADER*);
//void printAllFuncName(const char* fileName) {
//	LPWIN32_FIND_DATA lpwfd_first = new WIN32_FIND_DATA;//接受findfirstfile的结构指针
//	HANDLE hFile, hFileMap;//文件句柄和内存映射文件句柄
//	DWORD fileAttrib = 0;//存储文件属性用，在createfile中用到。
//	void* mod_base;//内存映射文件的起始地址，也是模块的起始地址
//
//
//	PFNEXPORTFUNC ImageRvaToVax = NULL;
//	HMODULE hModule = ::LoadLibrary("DbgHelp.dll");
//	if (hModule != NULL) {
//		ImageRvaToVax = (PFNEXPORTFUNC)::GetProcAddress(hModule, "ImageRvaToVa");
//		if (ImageRvaToVax == NULL) {
//			::FreeLibrary(hModule);
//			printf("取得函数失败\n");
//			return;
//		}
//
//	}
//	else {
//		printf("加载模块失败\n");
//		return;
//	}
//
//	if (FindFirstFile(fileName, lpwfd_first) == NULL) {//返回值为NULL，则文件不存在，退出
//		printf("文件不存在: %s ", fileName);
//		return;
//	}
//	else {
//		DWORD fileAttrib = lpwfd_first->dwFileAttributes;
//	}
//	hFile = CreateFile(fileName, GENERIC_READ, 0, 0, OPEN_EXISTING, fileAttrib, 0);
//	if (hFile == INVALID_HANDLE_VALUE) {
//		printf("打开文件出错！");
//		return;
//	}
//	hFileMap = CreateFileMapping(hFile, 0, PAGE_READONLY, 0, 0, 0);
//	if (hFileMap == NULL) {
//		CloseHandle(hFile);
//		printf("建立内存映射文件出错！");
//		return;
//	}
//	mod_base = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);
//	if (mod_base == NULL)
//	{
//		printf("建立内存映射文件出错！");
//		CloseHandle(hFileMap);
//		CloseHandle(hFile);
//		return;
//	}
//	IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER*)mod_base;
//	IMAGE_NT_HEADERS* pNtHeader =
//		(IMAGE_NT_HEADERS*)((BYTE*)mod_base + pDosHeader->e_lfanew);//得到NT头首址
//		//IMAGE_NT_HEADERS * pNtHeader =ImageNtHeader (mod_base);
//	IMAGE_OPTIONAL_HEADER* pOptHeader =
//		(IMAGE_OPTIONAL_HEADER*)((BYTE*)mod_base + pDosHeader->e_lfanew + 24);//optional头首址
//
//	IMAGE_EXPORT_DIRECTORY* pExportDesc = (IMAGE_EXPORT_DIRECTORY*)ImageRvaToVax(pNtHeader, mod_base, pOptHeader->DataDirectory[0].VirtualAddress, 0);
//	//导出表首址
//	PDWORD nameAddr = (PDWORD)ImageRvaToVax(pNtHeader, mod_base, pExportDesc->AddressOfNames, 0);//函数名称表首地址每个DWORD代表一个函数名字字符串的地址
//	PCHAR func_name = (PCHAR)ImageRvaToVax(pNtHeader, mod_base, (DWORD)nameAddr[0], 0);
//	DWORD i = 0;
//	DWORD unti = pExportDesc->NumberOfNames;
//	for (i = 0; i < unti; i++) {
//		printf("%s\n", func_name);
//		func_name = (PCHAR)ImageRvaToVax(pNtHeader, mod_base, (DWORD)nameAddr[i], 0);
//	}
//	CloseHandle(hFileMap);
//	CloseHandle(hFile);
//}
int main1()
{
	return 0;
	func function;
	double result;
	//printAllFuncName("C:\\Users\\ZY\\source\\repos\\第三阶段\\dll2\\Dll1\\Debug\\math.dll");
	HINSTANCE hinstLib;
	//hinstLib = LoadLibrary("C:\\Users\\ZY\\source\\repos\\第三阶段\\dll2\\Dll1\\Debug\\math.dll");//https://docs.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-6.0/aa270768(v=vs.60)
	//HINSTANCE hinstLib2 = LoadLibrary("C:\\Users\\ZY\\source\\repos\\第三阶段\\Dll3\\Debug\\strfunc.dll");//https://docs.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-6.0/aa270768(v=vs.60)

	//进程调用LoadLibrary（或AfxLoadLibrary）来显式链接到 DLL。如果成功，该函数会将指定的 DLL 映射到调用进程的地址空间，并返回 DLL 的句柄。
	//如果 DLL 具有入口点函数，则操作系统在调用LoadLibrary的线程的上下文中调用该函数。TODO应该是Dllmain中的内容
	/*if (hinstLib == NULL) {
		printf("ERROR:unable to load DLL\n");
		return 1;
	}*/
	funcGetF myFunc;
	funcGetN myFuncN;
	funcGetF myFuncEx;

	string s[2] = { "C:\\Users\\ZY\\source\\repos\\第三阶段\\dll2\\Dll1\\Debug\\math.dll" ,"C:\\Users\\ZY\\source\\repos\\第三阶段\\Dll3\\Debug\\strfunc.dll" };
	for (int j = 0; j < 2; j++)
	{
		hinstLib = LoadLibrary(s[j].c_str());
		if (hinstLib == NULL) {
			printf("ERROR:unable to load DLL\n");
			return 1;
		}
		myFunc = (funcGetF)GetProcAddress(hinstLib, "getName");
		myFuncN = (funcGetN)GetProcAddress(hinstLib, "getFuncN");
		myFuncEx = (funcGetF)GetProcAddress(hinstLib, "getExplain");
		const char** funcArr = myFuncEx();
		for (int i = 0; i < myFuncN(); i++)
		{
			cout << funcArr[i] << endl;
		}
		FreeLibrary(hinstLib);
	}

	//return 1;

	//函数名从何而来？无法从导入表中得到，
	/*
	1、手动每次写到主程序的某个.h文件中读取？
	2、放到.so中，作为字符串常量存在，去读取。（如何直接访问变量名，而不用通过函数获取?）
	3、动态加载后，通过获取so的导出表，或者主程序的导入表（但貌似导入表IAT没有被修改），需要更深入了解GetProcAddress的原因，DLL加载到内存中，其映射关系在哪里保存？如果不是IAT的话。
	目前选用第二种方法，但会造成其他人编写.so的规则要保持一致，不可没有这个函数名构成的字符串常量。
	而了解到c++并没有映射的机制，无法得到函数签名，但pe文件也就是dll文件格式并不了解，里面有存储的符号表的作用待研究
	*/
	//function = (func)GetProcAddress(hinstLib, "addFunc");//以后只用GetProcAddress来返回函数地址，而func声明的类型，可以用嵌入汇编代码完成统一操作
	//if (function == NULL)
	//{
	//	printf("ERROR:unable to find DLL function\n");
	//	FreeLibrary(hinstLib);
	//	return 1;
	//}
	//result = function(1, 2);
	//FreeLibrary(hinstLib);
	//cout << hex << result << endl;
	return 0;

}
