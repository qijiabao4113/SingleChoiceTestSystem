#pragma once
#ifndef _SYSTEMHEAD_
#define _SYSTEMHEAD_

#include <string>
#include <vector>
#include "testHead.h"

using std::string;
using std::vector;

//用户文件路径
const string pathUser = "D:/WorkSpace/VSFILE/SingleChoiceTestSystem/data/User.txt";
//题库文件路径
const string pathProblem = "D:/WorkSpace/VSFILE/SingleChoiceTestSystem/data/Problem.txt";


const string usertype = "user";					//用户身份标识
const string admintype = "administrators";		//管理员身份标识

bool registerNum(string& tempName);			//注册
bool logOn(string& Name, string& Password);				//登录
void systemMenu();			//主菜单
void unregisteredUserMenu(string& name, string& password); //未注册用户菜单
void exitSystem();			//退出系统
void systemSleep(int stime);			//延迟等待
bool judgmentString(string& str);		//判断是否为字符串
bool judgmentNumber(string& str);			//判断是否为数字
bool openMenu(string& userName, string& userType);		//开启菜单
bool returnMenu(string& userName, string& userType);	//返回上一级
string encryptedInput();					//加密输入
void Random(int* arr, int num, int minNum, int maxNum);//生成范围在minNum~maxNum的随机数 
int cmpfunc(const void* a, const void* b); //题目编号排序方式
char getUserAnswer();					//读取用户输入的答案
bool getProblemData(vector<CProblem>* data, int* maxNum); //获取题库数据
string strToLower(string& str);				//字符串转换成小写
#endif	//_SYSTEMHEAD_