#include "basicHead.h"
#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <cstdlib>

using std::cout;
using std::fstream;
using std::ios;
using std::map;
using std::pair;
using std::endl;


bool CBasic::changPassword(string& Name, string& oldPassword, string& newPassword, string path)
{
	string userName;			//用户名存储
	pair<string, string>userMessage;		//用户密码与身份信息存储
	map<string, pair<string, string>>mp;	//key:用户名 value:用户密码与身份信息
	fstream foi(path, ios::in);			//打开注册信息文件
	fstream fwoi;	
	if (!foi)				//文件异常处理
	{
		cout << "系统文件打开失败！请联系系统管理员。\n" << endl;
		system("pause");						//暂停
		exit(EXIT_FAILURE);				//系统异常终止
	}
	if (foi.peek() == EOF)			//文件空时特殊处理
	{
		cout << "系统文件空, 请联系系统管理员\n";
		return false;
	}
	while (!foi.eof())								//把文件遍历到文件尾
	{
		foi >> userName >> userMessage.first >> userMessage.second; //写入临时变量
		mp[userName] = userMessage;					//文件数据存入map
	}
	auto tmpit = mp.find(Name);				//查找在此用户信息

	if (tmpit == mp.end())					//未找到相关用户
		cout << "未找到相关用户\n";
	else if (tmpit->second.first == oldPassword) //新旧密码比对
	{
		tmpit->second.first = newPassword;		//替换旧密码
		fwoi.open(path, ios::out);			//打开注册信息文件
		for (auto it = mp.begin(); it != mp.end(); it++)	//写入更新后的数据
			fwoi << it->first << " " << it->second.first << " " << it->second.second << endl;
		fwoi.close();						//关闭文件
		cout << "密码修改成功\n";
	}
	else
		cout << "原密码错误，修改失败\n";
	foi.clear();						//更改cin状态标识符
	foi.close();						//关闭文件
	

	return false;
}

string CBasic::getUserName()
{
	return userName;
}

string CBasic::getpassword()
{
	return password;
}

string CBasic::getIdentity()
{
	return identity;
}

bool CBasic::cancelAccount(string instruct, string path, string Name)
{
	if (instruct == "no")		//撤销操作
	{
		cout << "操作已撤销\n";
		return false;
	}
	string userName;			//用户名存储
	pair<string, string>userMessage;		//用户密码与身份信息存储
	map<string, pair<string, string>>mp;	//key:用户名 value:用户密码与身份信息
	fstream foi(path, ios::in);			//打开注册信息文件
	if (!foi)				//文件异常处理
	{
		cout << "系统文件打开失败！请联系系统管理员。\n" << endl;
		system("pause");						//暂停
		exit(EXIT_FAILURE);				//系统异常终止
	}
	if (foi.peek() == EOF)			//文件空时特殊处理
	{
		cout << "系统文件空, 请联系系统管理员\n";
		return false;
	}
	while (!foi.eof())								//把文件遍历到文件尾
	{
		foi >> userName >> userMessage.first >> userMessage.second; //写入临时变量
		mp[userName] = userMessage;					//文件数据存入map
	}
	auto tmpit = mp.find(Name);				//查找在此用户信息

	if (tmpit == mp.end())					//未找到相关用户
		cout << "未找到用户文件，注销失败\n";
	else
	{
		mp.erase(Name);					//删除用户信息
		cout << "账号注销成功\n";
	}
	foi.clear();						//更改cin状态标识符
	foi.close();						//关闭文件
	foi.open(path, ios::out);			//打开注册信息文件
	for (auto it = mp.begin(); it != mp.end(); it++)	//写入更新后的数据
		foi << it->first << " " << it->second.first << " " << it->second.second << endl;
	foi.close();						//关闭文件

	return true;
}
