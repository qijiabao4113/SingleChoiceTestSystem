#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <utility>
#include <Windows.h>
#include "systemHead.h"
#include "errorExceptionHead.h"
#include "testHead.h"
#include <conio.h>
#include <algorithm>
#include <cctype>


using std::cout;
using std::cin;
using std::endl;
using std::pair;
using std::map;
using std::fstream;
using std::vector;
using std::ios;
using std::flush;
using std::fill;
using std::getline;

bool logOn(string& Name, string& Password)			//登录
{
	
	string userName;					//用户名
	pair<string, string>userMessage;		//用户密码与身份信息存储
	map<string, pair<string, string>>mp;		//key:用户名 value:用户密码与身份信息
	fstream foi(pathUser, ios::in);			//打开注册信息文件

	if (!foi)				//文件异常处理
	{
		cout << "用户文件打开失败！请联系系统管理员。\n" << endl;
		system("pause");						//暂停
		exitErrorTheSystem();				//系统异常终止
	}
	if (foi.peek() == EOF)			//文件空时特殊处理
	{
		cout << "系统文件空, 请联系系统管理员\n";
		return false;
	}
	while (!foi.eof())								//把文件遍历到文件尾
	{
		foi >> userName >> userMessage.first >> userMessage.second;
		mp[userName] = userMessage;					//文件数据存入map
	}
	int errortime = 0;						//密码输入错误次数
	bool flag = false;						//标记用户是否可以继续输入
	do
	{
		auto tmpit = mp.find(Name);				//查找在此用户信息

		if (tmpit == mp.end())					//未找到相关用户
		{
			flag = true;
			cout << "未注册用户,请先注册\n\n";
			systemSleep(2000);					//系统休眠
			cout << "加载中请稍后.....\n";
			unregisteredUserMenu(Name, Password);		//调用未注册用户菜单
		}
		else if (tmpit->second.first != Password)		//用户名正确存在密码错误
		{
			if (errortime > 2)						//监控密码错误次数
			{
				cout << "密码错误次数过多！！！\n五秒后退出系统。";
				for (int i = 5; i > 0; i--)
				{
					cout << endl << i << endl;
					systemSleep(1000);
				}
				exitSystem();
			}
			errortime++;					//记录密码错误次数
			cout << "密码错误\n请重新输入密码\n";
			cout << "您还有" << 4 - errortime << "次机会！";
			cout << "请输入密码\n";
			systemSleep(2000);			//系统休眠
			cin >> Password;			//键盘读取用户输入的密码
			flag = true;				//允许用户输入
		}
		else							//用户名和密码都正确
			openMenu(Name, tmpit->second.second);		//打开菜单
	} while (flag);					//达到登录条件跳出循环

	return !flag;
}

bool registerNum(string& tempName)		//注册
{
	string tempPassword;				//存储用户密码
	fstream foi(pathUser, ios::in | ios::app);			//打开注册信息文件
	string userName, userPassword, usertype;	//临时存储用户名，密码，用户类型
	vector<string>user;					//存储用户信息
	if (!foi)				//文件异常处理
	{
		cout << "用户文件打开失败！请联系系统管理员。\n" << endl;
		system("pause");						//暂停
		exitErrorTheSystem();				//系统异常终止
	}
	if (foi.peek() == EOF)			//文件空时特殊处理
	{
		cout << "系统文件空, 请联系系统管理员\n";
		return false;
	}
	while (!foi.eof())								//把文件遍历到文件尾
	{
		foi >> userName >> userPassword >> usertype;	//临时存储储用户名，密码，用户类型
		user.push_back(userName);					//文件数据存入vector
	}
	int flag = false;								//异常标记
	do
	{
		flag = false;								//初始化异常标记
		//find用判当前用户名是否被注册，find返回的是容器尾那么说明没有找到
		//find返回的不是容器尾那么说明此用户名已存在
		if (user.end() != find(user.begin(), user.end(), tempName))
		{
			flag = true;							//用户名已被占用标记异常
			cout << "此用户名已存在！请重新输入\n";
			systemSleep(2000);						//系统休眠
			cout << "请输入用户名\n";
			cin >> tempName;						//要求重新输入用户名
		}

	} while (flag);									//异常输入要求重新输入
	cout << "请输入密码\n";
	tempPassword = encryptedInput();		//键盘读取用户输入,加密输入
	foi.clear();									//更改cin状态标识符
	CUser tempUsr;
	foi << tempName << " " << tempPassword << " " << tempUsr.getIdentity() << endl;		//给文件追加内容，保存注册信息
	foi.close();									//关闭文件
	return !flag;
}

void systemMenu()		//主菜单
{
	system("cls");
	cout << "|------------------------------------------------|\n";
	cout << "|                                                |\n";
	cout << "|            ***单项选择题测试系统***            |\n";
	cout << "|                                                |\n";
	cout << "|================================================|\n";
	cout << "|                                                |\n";
	cout << "|=                 1.注册账号                   =|\n";
	cout << "|=                 2.登录系统                   =|\n";
	cout << "|=                 3.退出系统                   =|\n";
	cout << "|                                                |\n";
	cout << "|================================================|\n";
}

void unregisteredUserMenu(string& name, string& password)
{
	bool flag = false;							//异常标记
	int sel = -1;    							//菜单选项
	do
	{
		system("cls");
		cout << "================================================\n";
		cout << endl;
		cout << "=                 1.重新输入                   =\n";
		cout << "=                 2.注册账号                   =\n";
		cout << "=                 3.退出系统                   =\n";
		cout << endl;
		cout << "================================================\n";
		flag = 0;							//未注册用户菜单选项标记初始化
		cin >> sel;							//用户键盘输入选项
		switch (sel)
		{
		case 1:
			cout << "请输入用户名\n";
			cin >> name;					//用户重新从键盘输入用户名
			cout << "请输入密码\n";
			password = encryptedInput();		//键盘读取用户输入,加密输入
			break;
		case 2:
			cout << "请输入用户名\n";
			cin >> name;					//用户键盘输入选项用户名
			registerNum(name);				//调用注册函数
			break;
		case 3: exitSystem();				//退出系统
			break;
		default: flag = true;					//标记非法输入
		}
		if (flag)						//非法输入
		{
			cout << "无效输入，请重新输入.\n";
			cin.clear();			//更改cin状态标识符准备接受下一次的输入
			systemSleep(1000);				//系统休眠
			refreshBuffer();				//刷新缓冲区
		}
	} while (flag);					//非法输入要求重新输入
}

void exitSystem()
{
	//程序正常退出
	exit(EXIT_SUCCESS);
}

void systemSleep(int stime)	//延迟等待
{
	//系统休眠
	Sleep(stime);
}

bool judgmentString(string& str)
{
	//遍历字符串
	for (int i = 0; i < str.size(); i++)
		if (!isalpha(str[i]))				//逐个字符判断是否为字母
			return false;					//发现非字母返回false
	return true;
}

bool judgmentNumber(string& str)
{
	//遍历字符串
	for (int i = 0; i < str.size(); i++)
		if (!isdigit(str[i]))				//逐个字符判断是否为数字
			return false;					//发现非数字返回false
	return true;
}

bool openMenu(string& userName, string& userType)
{
	cout << "拼命加载中......、\n";
	systemSleep(1500);						//系统休眠
	CAdmin admin;
	CUser user;							
	string tmpUserName;					//临时存储用户名
	string inOldPassword, inNewPassword;	//临时存储新旧密码
	string problemNum;				//题目序号
	CProblem tmpProblem;		//临时存储题目数据
	string tmpNum, tmpTitle, tmpA, tmpB, tmpC, tmpD, tmpAnswer; //临时存储题目信息
	string ans = "no";							//记录用户输入和相关命令
	bool flag1 = false;						    //标记选项1合法性
	bool flag2 = false;						    //标记选项2合法性
	bool flag3 = false;						    //标记选项3合法性
	bool flag4 = false;						    //标记选项4合法性
	bool flag5 = false;						    //标记选项5合法性
	bool flag7 = false;						    //标记选项7合法性
	if (userType == admin.getIdentity())		//判断是否为管理员
	{
		admin.menu();					//管理员菜单
		bool flag_admin = false;				//标记管理员菜单选项的选择合法性		
		int sel = -1;						//菜单选项
		//题目编号，标题，选项A,选项B,选项C,选项D
		do
		{
			flag_admin = false;				//管理员菜单选项标记初始化
			cin >> sel;					//用户键盘输入选项
			refreshBuffer();	//清空缓冲区
			switch (sel)
			{
			case 1:	
				admin.viewQuestions();	//调用查看题库函数
				returnMenu(userName, userType);		//返回上一级菜单
				break;
			case 2:
				tmpProblem.setNum("0");		//默认题目编号为0
				cout << "请输入题目标题：\n";
				getline(cin, tmpTitle);		//键盘读取题目标题
				tmpProblem.setTitle(tmpTitle);
				cout << "请输入选项A内容：\n";
				getline(cin,tmpA);				//键盘读取选项A内容
				tmpProblem.setA("A :" + tmpA);	//给选项加入A前缀
				cout << "请输入选项B内容：\n";
				getline(cin, tmpB);				//键盘读取选项B内容
				tmpProblem.setB("B :" + tmpB);	//给选项加入B前缀
				cout << "请输入选项C内容：\n";
				getline(cin, tmpC);				//键盘读取选项C内容
				tmpProblem.setC("C :" + tmpC);	//给选项加入C前缀
				cout << "请输入选项D内容：\n";
				getline(cin, tmpD);				//键盘读取选项D内容
				tmpProblem.setD("D :" + tmpD);	//给选项加入D前缀
				cout << "请输入标准答案(A(a)/B(b)/C(c)/D(d))：\n";
				tmpAnswer = getUserAnswer();	//键盘读取标准答案,char可以赋值给string
				tmpProblem.setAnswer("标准答案 :" + tmpAnswer); //给标准答案加入前缀
				admin.addQuestions(tmpProblem);	//调用增加题目的函数
				returnMenu(userName, userType);		//返回上一级菜单
				break;
			case 3:
				do
				{
					flag3 = false;			//选项三内部标记初始化
					cout << "请输入需要删除的题目编号：\n";
					cin >> problemNum;		//用户键盘输入题目编号
					if (!judgmentNumber(problemNum))	//判断输入是否为合法数字
					{
						cout << "无效输入，请重新输入。\n";
						flag3 = true;		//标记非法
						refreshBuffer();	//清空缓冲区
					}
				} while (flag3);			//非法要求重新输入
				admin.deleteQuestions(problemNum);	//调用删除题目函数
				returnMenu(userName, userType);		//返回上一级菜单
				break;
			case 4:	
				cout << "请输入被操作的用户名：\n";
				cin >> tmpUserName;
				admin.userManagement(tmpUserName, admin, userName);
				returnMenu(userName, userType);		//返回上一级菜单
				break;
			case 5:
				do
				{
					ans = "no";						//确保安全性默认指令为no
					flag5 = false;					//选项二内部标记初始化
					cout << "是否确认清空，请输入yes/no\n";
					cin >> ans;						//用户键盘输入命令
					ans = strToLower(ans);			//统一转换为小写
					if (!(ans == "yes" || ans == "no"))
					{
						cout << "无效输入，请重新输入。\n";
						flag5 = true;
					}
				} while (flag5);					//非法输入要求重新输入
				admin.clearQuestions(ans, pathProblem);
				returnMenu(userName, userType);		//返回上一级菜单
				break;
			case 6:
				cout << "请输入旧密码：";
				inOldPassword = encryptedInput();			//键盘输入旧密码
				cout << "请输入新密码：";
				inNewPassword = encryptedInput();			//键盘输入新密码
				//调用更改密码函数
				admin.changPassword(userName, inOldPassword, inNewPassword, pathUser);
				returnMenu(userName, userType);		//返回上一级菜单
				break;
			case 7:
				do
				{
					ans = "no";						//确保安全性默认指令为no
					flag7 = false;					//选项二内部标记初始化
					cout << "是否确认注销，请输入yes/no\n";
					cin >> ans;						//用户键盘输入命令
					ans = strToLower(ans);			//统一转换为小写
					if (!(ans == "yes" || ans == "no"))
					{
						cout << "无效输入，请重新输入。\n";
						flag7 = true;
					}
				} while (flag7);					//非法输入要求重新输入
				admin.cancelAccount(ans, pathUser, userName);	//调用账号注销函数
				returnMenu(userName, userType);		//返回上一级菜单
				break;
			case 8: exitSystem();			//退出系统
				break;
			default: flag_admin = true;		//非法输入会改变合法输入标记
			}
			if (flag_admin)		//非法输入
			{
				cout << "无效输入，请重新输入.\n";
				cin.clear();			//更改cin状态标识符准备接受下一次的输入
				systemSleep(1000);		//系统休眠
				refreshBuffer();		//刷新缓冲区
			}
		} while (flag_admin);			//非法输入要求重新输入
	}
	else if (userType == user.getIdentity())	//判断是否为用户
	{
		user.menu();								//用户菜单 
		bool flag_user = false;							//标记用户菜单选项的选择合法性
		string testNum;								//用户输入的题目抽取数量
		int sel = -1;									//菜单选项
		do
		{
			flag_user = false;							//用户菜单选项标记初始化
			cin >> sel;								//用户键盘输入选项
			switch (sel)
			{
			case 1:
				do
				{
					flag1 = false;					//选项一内部标记初始化
					cout << "请输入抽题数量：\n";
					cin >> testNum;					//用户键盘输入抽题数量	
					if (!judgmentNumber(testNum))	//判断是否合法数字
					{
						cout << "无效输入，请重新输入。\n";
						flag1 = true;
					}
				} while (flag1);					//非法输入要求重新输入				
				user.testQuestions(testNum);
				returnMenu(userName, userType);		//返回上一级菜单
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
				user.cancelAccount(ans, pathUser, userName);	//调用账号注销函数
				returnMenu(userName, userType);		//返回上一级菜单
				break;
			case 3:
				cout << "请输入旧密码：";
				cin >> inOldPassword;			//键盘输入新密码
				cout << "请输入新密码：";
				cin >> inNewPassword;			//键盘输入旧密码
				//调用更改密码函数
				user.changPassword(userName, inOldPassword, inNewPassword, pathUser);
				returnMenu(userName, userType);		//返回上一级菜单
				break;
			case 4:	exitSystem();					//退出系统
				break;
			default: flag_user = true;						//非法输入会改变合法输入标记
			}
			if (flag_user)				//非法输入
			{
				cout << "无效输入，请重新输入.\n";
				cin.clear();			//更改cin状态标识符准备接受下一次的输入
				systemSleep(1000);		//系统休眠
				refreshBuffer();		//刷新缓冲区
			}
		} while (flag_user);			//非法输入要求重新输入
	}
	else	//未知身份	
	{
		unknownIdentity();
	}

	return true;
}

bool returnMenu(string& userName, string& userType)
{
	bool reMenuFlag = false;		//用于标记非法输入
	system("pause");				//清屏前暂停给用户阅读屏幕的机会
	do
	{
		system("cls");				//清屏
		cout << "================================================\n";
		cout << endl;
		cout << "=                 1.返回上级                   =\n";
		cout << "=                 2.退出系统                   =\n";
		cout << endl;
		cout << "================================================\n";
		int sel = -1;				//菜单选项
		cin >> sel;					//用户键盘输入选项
		switch (sel)
		{
		case 1:
			openMenu(userName, userType);	//调用自身达到重开菜单的目的
			break;
		case 2:
			exitSystem();			//调用退出系统函数
			break;
		default:
			reMenuFlag = true;		//标记非法输入
		}
		if (reMenuFlag)				//非法输入
		{
			cout << "无效输入，请重新输入.\n";
			cin.clear();			//更改cin状态标识符准备接受下一次的输入
			systemSleep(1000);		//系统休眠
			refreshBuffer();		//刷新缓冲区
		}
	} while (reMenuFlag);	//非法输入要求重新输入

	return !reMenuFlag;
}

string encryptedInput()
{
	string str = "";		//空字符串
	char init = '\0';		//默认字符为空
	for (;;)
	{
		init = _getch();		//获取输入但不显示
		if (init == VK_RETURN)   //如果键入的是Enter 键那么换行并结束输入
		{
			cout << endl;
			break;
		}
		else if (init == VK_BACK) //如果键入的是BACKSPACE 键那么进行退回
		{
			cout << "\b \b" << flush;
			str.erase(str.length() - 1);
		}
		else					//正常输入时打印*号
		{
			cout << "*" << flush;
			str += init;		//把字符接入字符串
		}
	}

	return str;
}

void Random(int* arr, int num, int minNum, int maxNum)//生成范围在minNum~maxNum的随机数 
{
	if (num == maxNum)	//抽取数量与题目数量相等时不需要随机抽取直接全部输出
	{
		for (int i = 0; i < num; i++)
			arr[i] = i + 1;
		return;
	}
	srand(time(NULL));  //设置时间种子
	bool* flagArr = new bool[maxNum + 1];		//用于标记已经出现的随机数
	fill(flagArr, flagArr + maxNum + 1, false);		//标记数组初始化为FALSE
	int temp = 0;				//临时保存随机数
	for (int i = 0; i < num;) 
	{
		temp = rand() % (maxNum - minNum + 1) + minNum;//生成区间maxNum~l的随机数 
		if (!flagArr[temp])		//保证数字唯一性
		{
			flagArr[temp] = true;		//下标为数字，元素值为标记，每次发现新数则标记
			arr[i] = temp;		//保存数字
			i++;		//发现新数字并保存后随机数数组下标后移
		}
		else			//不是新数字就跳过
			continue;
	}
}

int cmpfunc(const void* a, const void* b)
{
	//从小到大排序
	return (*(int*)a - *(int*)b);
}

char getUserAnswer()
{
	string tmpStr = "";
	char ans = '\0';
	bool flag = false;		//用于标记非法输入
	
	do
	{
		flag = false;				//每次输入前都需要重置标记
		cin >> tmpStr;				//接收所有输入
		if (tmpStr.size() > 1)		//多字符属于非法
			flag = true;			//标记非法
		else
			ans = toupper(tmpStr[0]);		//统一转换为大写方便后续非法判断
		//非法判断
		if (!(ans == 'A' || ans == 'B' || ans == 'C' || ans == 'D') || flag)
		{
			cout << "非法输入！请重新输入：\n";
			cin.clear();					//重置cin状态，准备下一次输入
			flag = true;			//标记非法
		}

	} while (flag);		//非法要求重新输入

	return ans;				//返回用户输入的答案
}

bool getProblemData(vector<CProblem>* data, int* maxNum)
{
	CProblem tmpProblem;				//临时存储题目数据
	//临时存储题目编号，标题，选项A,选项B,选项C,选项D,答案
	string tmpNum, tmpTitle, tmpA, tmpB, tmpC, tmpD, tmpAnswer;
	//vector<CProblem>vccp;				//临时存储题库文件内容
	fstream foi(pathProblem, ios::in | ios::out);		//打开题库文件
	if (!foi)				//文件异常处理
	{
		cout << "用户文件打开失败！请联系系统管理员。\n" << endl;
		system("pause");						//暂停
		exitErrorTheSystem();				//系统异常终止
		return false;
	}
	if (foi.peek() == EOF)					//文件空时特殊处理
	{
		foi.clear();						//更改cin状态标识符
		cout << "题库文件空！请联系系统管理员添加题库。\n" << endl;
		foi.close(); //关闭文件
		return true;
	}
	while (!foi.eof())					//遍历文件
	{
		getline(foi, tmpNum);			//获取题目编号
		getline(foi, tmpTitle);			//获取题目标题
		getline(foi, tmpA);				//获取选项A
		getline(foi, tmpB);				//获取选项B
		getline(foi, tmpC);				//获取选项C
		getline(foi, tmpD);				//获取选项D
		getline(foi, tmpAnswer);		//获取答案
		tmpProblem.setNum(tmpNum);		//临时存储题目编号
		tmpProblem.setTitle(tmpTitle);	//临时存储题目标题
		tmpProblem.setA(tmpA);			//临时存储选项A
		tmpProblem.setB(tmpB);			//临时存储选项B
		tmpProblem.setC(tmpC);			//临时存储选项C
		tmpProblem.setD(tmpD);			//临时存储选项D
		tmpProblem.setAnswer(tmpAnswer);//临时存储标准答案
		data->push_back(tmpProblem);		//题目内容存入vetor容器
	}
	foi.close(); //关闭文件
	tmpProblem = data->back();			//获取题库最后一个题目
	*maxNum = stoi((tmpProblem.getNum()));	//最大题目编号

	return true;
}

string strToLower(string& str)
{
	for (string::iterator it = str.begin(); it != str.end(); it++)
		*it = tolower(*it);
	return str;
}
