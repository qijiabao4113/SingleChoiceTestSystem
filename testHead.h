#pragma once
#include <string>
#include "basicHead.h"

class CAdmin;
class CUser;
class Cproblem;

using std::string;

#ifndef _TESTHEAD_
#define _TESTHEAD_

class CProblem
{
private:
	string num;				//题目编号
	string title;			//题目
	string A, B, C, D;		//各选项答案
	string isAnswer;		//题目答案

public:
	char answer = ' ';			//用户回答的答案
	bool setNum(string _Num);
	bool setTitle(string _title);
	bool setA(string _A);
	bool setB(string _B);
	bool setC(string _C);
	bool setD(string _D);
	bool setAnswer(string _isAnswer);
	string getNum();
	string getTitle();
	string getA();
	string getB();
	string getC();
	string getD();
	string getAnswer();
};

class CAdmin:public CBasic
{

public:

	void menu();
	CAdmin();
	bool viewQuestions();				//查看题库 
	bool addQuestions(CProblem problemData);				//增加题目
	bool deleteQuestions(string num);				//删减题目
	void userManagement(string userName, CAdmin admin, string adminName);				//用户管理菜单	
	bool resettingUserPassword(string Name, string newPassword, string path);	//重置用户密码
	bool clearQuestions(string instruct, string path);				//清空题库
};

class CUser:public CBasic
{

public:

	void menu();
	CUser();
	bool testQuestions(string num);				//开始测试
};

#endif //_TESTHEAD_