#include <cstdlib>
#include "errorExceptionHead.h"
#include <iostream>

using std::cout;
//异常退出系统
void exitErrorTheSystem()
{
	//程序异常退出
	exit(EXIT_FAILURE);
}

//身份验证异常
void unknownIdentity()
{
	cout << "未知身份，登录失败，请联系统管理员\n";
}

//刷新缓冲区
void refreshBuffer()
{
	//将文件指针重新指向文件开头,达到刷新缓冲区效果
	rewind(stdin);
}