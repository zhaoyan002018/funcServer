// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"
//结果上看只需要对声明部分进行C声明即可

double addFunc(double a, double b)
{
	return a + b;
}
double delFunc(double a, double b)
{
	return a - b;
}
double mulFunc(double a, double b)
{
	return a * b;
}
int divFunc(int a, int b)
{
	return a / b;
}


// 当使用预编译的头时，需要使用此源文件，编译才能成功。
