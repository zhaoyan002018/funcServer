﻿// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//printf("DLL_PROCESS_ATTACH\n");
	case DLL_THREAD_ATTACH:
		//printf("DLL_THREAD_ATTACH\n");
	case DLL_THREAD_DETACH:
		//printf("DLL_THREAD_DETACH\n");
	case DLL_PROCESS_DETACH:
		//printf("DLL_PROCESS_DETACH\n");
		break;
	}
	return TRUE;
}
