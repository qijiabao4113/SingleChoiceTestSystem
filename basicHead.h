#pragma once

#include <string>

class CBasic;

using std::string;

#ifndef _BASICHEAD_
#define _BASICHEAD_

class CBasic
{
protected:
	string identity;		//身份标签
	string userName;		//用户名
	string password;		//密码
public:
	bool changPassword(string& Name, string& oldPassword, string& newPassword, string path);		//修改密码
	string getUserName();		//获取用户名
	string getpassword();		//获取密码
	string getIdentity();		//获取身份标签
	bool cancelAccount(string instruct, string path, string Name);	//注销账户
};

#endif	//_BASICHEAD_