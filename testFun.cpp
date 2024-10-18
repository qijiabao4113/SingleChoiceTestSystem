#include "testHead.h"
#include <iostream>
#include <fstream>
#include "systemHead.h"
#include <vector>
#include <string>
#include "errorExceptionHead.h"
#include <algorithm>
#include <map>
#include <utility>
#include <cstdlib>

using std::cout;
using std::cin;
using std::endl;
using std::fstream;
using std::ios;
using std::vector;
using std::getline;
using std::stoi;
using std::to_string;
using std::find;
using std::map;
using std::pair;
using std::sort;
using std::size_t;

void CAdmin::menu()
{
	system("cls");
	//管理员菜单
	cout << "|------------------------------------------------|\n";
	cout << "|                                                |\n";
	cout << "|    ***欢迎登录单项选择题测试系统管理后台***    |\n";
	cout << "|                                                |\n";
	cout << "|================================================|\n";
	cout << "|                                                |\n";
	cout << "|=                 1.查看题库                   =|\n";
	cout << "|=                 2.增加题目                   =|\n";
	cout << "|=                 3.删减题目                   =|\n";
	cout << "|=                 4.用户管理                   =|\n";
	cout << "|=                 5.清空题库                   =|\n";
	cout << "|=                 6.修改密码                   =|\n";
	cout << "|=                 7.注销账号                   =|\n";
	cout << "|=                 8.退出系统                   =|\n";
	cout << "|                                                |\n";
	cout << "|================================================|\n";
}

CAdmin::CAdmin()
{
	identity = admintype;		//初始化管理员标签
}

bool CAdmin::viewQuestions()
{
	vector<CProblem>vccp;				//临时存储题库文件内容

	int maxNum = 0;
	int* ptrmaxNum = &maxNum;
	vector<CProblem>* ptrvccp = &vccp;
	getProblemData(ptrvccp, ptrmaxNum);
	cout << "----------------------------------------------\n\n";
	for (auto it = vccp.begin(); it != vccp.end(); it++)	//遍历vetor容器打印题库
	{
		cout << it->getNum() << " ";
		cout << it->getTitle() << endl;
		cout << it->getA() << endl;
		cout << it->getB() << endl;
		cout << it->getC() << endl;
		cout << it->getD() << endl;
		cout << it->getAnswer() << endl << endl;
	}

	cout << "总共有" << maxNum << "个题目\n";
	return true;
}

bool CAdmin::addQuestions(CProblem problemData)
{
	fstream foi(pathProblem, ios::in);		//打开题库文件
	if (!foi)				//文件异常处理
	{
		cout << "用户文件打开失败！请联系系统管理员。\n" << endl;
		system("pause");						//暂停
		exitErrorTheSystem();				//系统异常终止
		return false;
	}
	if (foi.peek() == EOF)					//文件空时特殊处理
	{
		foi.close();
		foi.open(pathProblem, ios::out);		//打开题库文件
		if (!foi)				//文件异常处理
		{
			cout << "用户文件打开失败！请联系系统管理员。\n" << endl;
			system("pause");						//暂停
			exitErrorTheSystem();				//系统异常终止
			return false;
		}
		problemData.setNum("1");
		foi << problemData.getNum() << endl;
		foi << problemData.getTitle() << endl;
		foi << problemData.getA() << endl;
		foi << problemData.getB() << endl;
		foi << problemData.getC() << endl;
		foi << problemData.getD() << endl;
		foi << problemData.getAnswer();
		foi.close(); //关闭文件
		cout << "添加成功!" << endl;
		return true;
	}
	foi.close();

	//文件不为空时
	foi.open(pathProblem, ios::app);		//打开题库文件
	if (!foi)				//文件异常处理
	{
		cout << "用户文件打开失败！请联系系统管理员。\n" << endl;
		system("pause");						//暂停
		exitErrorTheSystem();				//系统异常终止
		return false;
	}
	vector<CProblem>vccp;				//临时存储题库文件内容
	int maxNum = 0;
	int* ptrmaxNum = &maxNum;
	vector<CProblem>* ptrvccp = &vccp;
	getProblemData(ptrvccp, ptrmaxNum);
	//防止题目重复添加
	for (auto it = ptrvccp->begin(); it != ptrvccp->end(); it++)
		if (it->getAnswer() == problemData.getAnswer())
		{
			cout << "当前题目已经在题库中不需要重复添加！\n";
			return false;
		}
	//根据最后题目编号更新当前题目编号
	problemData.setNum(to_string(maxNum + 1));
	foi.clear();						//更改cin状态标识符
	//题目数据写入文件
	foi << endl << problemData.getNum() << endl;
	foi << problemData.getTitle() << endl;
	foi << problemData.getA() << endl;
	foi << problemData.getB() << endl;
	foi << problemData.getC() << endl;
	foi << problemData.getD() << endl;
	foi << problemData.getAnswer();
	foi.close(); //关闭文件
	cout << "添加成功!" << endl;
	return true;
}

bool CAdmin::deleteQuestions(string num)
{
	vector<CProblem>vccp;				//临时存储题库文件内容
	int maxNum = 0;
	int* ptrmaxNum = &maxNum;
	vector<CProblem>* ptrvccp = &vccp;
	getProblemData(ptrvccp, ptrmaxNum);

	if (stoi(num) > maxNum || stoi(num) < 0) //题目编号合法判断
		cout << "题目编号越界，删除失败\n";
	else
	{	
		//擦除相关编题目，vector下标是从0开始的，题目编号是从1开始的所以需要-1
		vccp.erase(vccp.begin() + stoi(num) - 1);	
		fstream foi(pathProblem, ios::out);
		if (!foi)				//文件异常处理
		{
			cout << "用户文件打开失败！请联系系统管理员。\n" << endl;
			system("pause");						//暂停
			exitErrorTheSystem();				//系统异常终止
			return false;
		}
		for (int i = 0; i < vccp.size(); i++)
		{
			//题目数据写入文件
			//i从0开始的，题目下标是从1开始的所以+1
			if (i != 0)
				foi << endl;
			foi << i + 1 << endl;
			foi << vccp[i].getTitle() << endl;
			foi << vccp[i].getA() << endl;
			foi << vccp[i].getB() << endl;
			foi << vccp[i].getC() << endl;
			foi << vccp[i].getD() << endl;
			foi << vccp[i].getAnswer();
		}
		foi.close();
		cout << "清除完毕\n";
	}

	return true;
}

void CAdmin::userManagement(string userName, CAdmin admin, string adminName)
{
	system("cls");
	//用户管理菜单
	cout << "================================================\n";
	cout << endl;
	cout << "=                 1.重置密码                   =\n";
	cout << "=                 2.删除账户                   =\n";
	cout << "=                 3.返回上级                   =\n";
	cout << "=                 4.退出系统                   =\n";
	cout << endl;
	cout << "================================================\n";
	
	string ans = "no";							//记录用户输入和相关命令
	bool flag2 = false;						//标记选项2合法性
	string inNewPassword;					//新密码
	bool flag_userManagement = false;				//标记用户管理菜单选项的选择合法性
	int sel = -1;									//菜单选项
	do
	{
		flag_userManagement = false;							//用户菜单管理选项标记初始化
		cin >> sel;								//用户键盘输入选项
		switch (sel)
		{
		case 1:
			cout << "请输入新密码：";
			inNewPassword = encryptedInput();			//键盘输入旧密码
			//调用更改密码函数
			system("cls");
			admin.resettingUserPassword(userName,inNewPassword, pathUser);
			cout << "三秒后自动返回上一级菜单。\n";
			for (int i = 3; i > 0; i--)
			{
				cout << endl << i << endl;
				systemSleep(1000);
			}
			admin.userManagement(userName, admin, userName);		//自动返回上一级菜单
			break;
			
		case 2:
			do
			{
				ans = "no";						//确保安全性默认指令为no
				flag2 = false;					//选项二内部标记初始化
				cout << "是否确认注销，请输入yes/no\n";
				cin >> ans;						//用户键盘输入命令
				ans = strToLower(ans);			//统一转换为小写
				if (!(ans == "yes" || ans == "no"))
				{
					cout << "无效输入，请重新输入。\n";
					flag2 = true;
				}
			} while (flag2);					//非法输入要求重新输入
			system("cls");
			admin.cancelAccount(ans, pathUser, userName);	//调用账号注销函数
			cout << "三秒后自动返回上一级菜单。\n";
			for (int i = 3; i > 0; i--)
			{
				cout << endl << i << endl;
				systemSleep(1000);
			}
			admin.userManagement(userName, admin, userName);		//自动返回上一级菜单
			break;
		case 3:	openMenu(adminName, admin.identity);		//返回上一级菜单
			break;
		case 4:	exitSystem();					//退出系统
			break;
		default: flag_userManagement = true;						//非法输入会改变合法输入标记
		}
		if (flag_userManagement)				//非法输入
		{
			cout << "无效输入，请重新输入.\n";
			cin.clear();			//更改cin状态标识符准备接受下一次的输入
			systemSleep(1000);		//系统休眠
			refreshBuffer();		//刷新缓冲区
		}
	} while (flag_userManagement);			//非法输入要求重新输入
}

bool CAdmin::resettingUserPassword(string Name, string newPassword, string path)
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
	else    //重置密码不需要新旧密码比对
	{
		tmpit->second.first = newPassword;		//替换旧密码
		fwoi.open(path, ios::out);			//打开注册信息文件
		for (auto it = mp.begin(); it != mp.end(); it++)	//写入更新后的数据
			fwoi << it->first << " " << it->second.first << " " << it->second.second << endl;
		fwoi.close();						//关闭文件
		cout << "密码重置成功\n";
	}
	foi.clear();						//更改cin状态标识符
	foi.close();						//关闭文件

	return false;
}

bool CAdmin::clearQuestions(string instruct, string path)
{
	if (instruct == "no")		//撤销操作
	{
		cout << "操作已撤销\n";
		return false;
	}
	fstream foi(path, ios::out || ios :: in);			//打开注册信息文件
	if (!foi)				//文件异常处理
	{
		cout << "系统文件打开失败！请联系系统管理员。\n" << endl;
		system("pause");						//暂停
		exit(EXIT_FAILURE);				//系统异常终止
	}
	if (foi.peek() == EOF)			//文件空时特殊处理
	{
		cout << "系统文件空, 不需要清空\n";
		return false;
	}
	foi.close();
	foi.open(path, ios::out);			//打开注册信息文件
	if (!foi)				//文件异常处理
	{
		cout << "系统文件打开失败！请联系系统管理员。\n" << endl;
		system("pause");						//暂停
		exit(EXIT_FAILURE);				//系统异常终止
	}
	foi << "";
	cout << "题库已清空\n";
	foi.close();
	return false;
}

void CUser::menu()
{
	system("cls");
	//用户菜单
	cout << "|------------------------------------------------|\n";
	cout << "|                                                |\n";
	cout << "|        ***欢迎登录单项选择题测试系统***        |\n";
	cout << "|                                                |\n";
	cout << "|================================================|\n";
	cout << "|                                                |\n";
	cout << "|=                 1.开始测试                   =|\n";
	cout << "|=                 2.注销账户                   =|\n";
	cout << "|=                 3.修改密码                   =|\n";
	cout << "|=                 4.退出系统                   =|\n";
	cout << "|                                                |\n";
	cout << "|================================================|\n";
}

CUser::CUser()
{
	identity = usertype;			//初始化用户标签
}

bool CUser::testQuestions(string num)
{
	vector<CProblem>vccp;				//临时存储题库文件内容
	int maxNum = 0;	   	               //最大题目编号
	int* ptrmaxNum = &maxNum;
	vector<CProblem>* ptrvccp = &vccp;
	getProblemData(ptrvccp, ptrmaxNum);
	int n = stoi(num);			//数组元素的个数，即生成随机数的个数
	//题目抽取数量合法判断
	if (n > maxNum || n < 0)	
	{
		cout << "抽取数据数量非法\n";
		return false;
	}
	if (0 == n)
	{
		cout << "抽取数据数量为0，系统未抽取题目\n";
		return false;
	}
	int* numArr = new int[n];	//保存随机抽取的题目编号
	Random(numArr, n, 1, maxNum);	//生成随机数的通常范围为0~32767，这里通过取模控制取值为0~maxNum 
	qsort(numArr, n, sizeof(*numArr), cmpfunc);		//库函数排序
	double socer = 0.0;
	double oneSocer = 100.0 / maxNum;

	int i = 0;
	for (auto it = vccp.begin(); it != vccp.end(); it++)
	{
		if (numArr[i] == stoi(it->getNum()))
		{
			i++;							//发现正确编号题目后编号数组才更新
			cout << it->getNum() << " ";
			cout << it->getTitle() << endl;
			cout << it->getA() << endl;
			cout << it->getB() << endl;
			cout << it->getC() << endl;
			cout << it->getD() << endl;
			cout << "请输入你的答案：\n";
			char ans = getUserAnswer();
			cout << it->getAnswer() << endl;
			cout << "你的答案为：" << ans << endl;
			if (ans == it->getAnswer().back())
			{
				cout << "你的回答是正确的\n";
				cout << "得分 + " << oneSocer << endl;
				socer += oneSocer;
				cout << "总得分 ：" << socer << endl;
			}
			else
				cout << "很遗憾，回答错误，再接再厉！\n";
			cout << endl;
		}
	}
	cout << "最终得分 ：" << socer << endl;

	return false;
}

bool CProblem::setNum(string _Num)
{
	num = _Num;
	return true;
}

bool CProblem::setTitle(string _title)
{
	title = _title;
	return true;
}

bool CProblem::setA(string _A)
{
	A = _A;
	return true;
}

bool CProblem::setB(string _B)
{
	B = _B;
	return true;
}

bool CProblem::setC(string _C)
{
	C = _C;
	return true;
}

bool CProblem::setD(string _D)
{
	D = _D;
	return true;
}

bool CProblem::setAnswer(string _isAnswer)
{
	isAnswer = _isAnswer;
	return true;
}

string CProblem::getNum()
{
	return num;
}

string CProblem::getTitle()
{
	return title;
}

string CProblem::getA()
{
	return A;
}

string CProblem::getB()
{
	return B;
}

string CProblem::getC()
{
	return C;
}

string CProblem::getD()
{
	return D;
}

string CProblem::getAnswer()
{
	return isAnswer;
}
