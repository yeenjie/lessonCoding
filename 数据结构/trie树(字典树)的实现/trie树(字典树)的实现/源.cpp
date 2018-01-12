#include<iostream>
#include<string>
#include <fstream> 
#include <cctype>
#include<windows.h> 
#include<iomanip>
#define ALP_NUM 28
using namespace std;
const int OK = 1;
int filecount = 0;
typedef struct prietree {
	char character;
	bool terminal;
	bool exist;
	int count;
	string str;
	struct prietree * children[ALP_NUM];
}Trietree, *TrietreeNode;


TrietreeNode TrieInit()
{
	TrietreeNode p;
	p = new(Trietree);  //用malloc申请空间的话 string就没有初始化了
	p->terminal = false;
	p->exist = false;
	for (int i = 0; i < ALP_NUM; i++)
	{
		p->children[i] = NULL;
	}
	p->count = 0;
	return p;
}

void windows();


int hideHandle() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);//获取光标信息
	cci.bVisible = FALSE;//隐藏光标
	SetConsoleCursorInfo(hOut, &cci);//设置光标信息
	return 0;
}


int TrieInsert(TrietreeNode & T, string InsStr)
{   //插入一个单词
	TrietreeNode InsNode = T;
	for (unsigned int i = 0; i < InsStr.length(); i++)
	{
		system("cls");
		if (InsStr[i] == ' ') {
			break;
		}
		cout << "正在加载中..." << endl;
	//	cout << "我正在插入" << InsStr[i] << endl;
		//	system("pause");
		if (InsNode->children[InsStr[i] - 'a'] == NULL)
		{
			InsNode->children[InsStr[i] - 'a'] = TrieInit();
		}
		InsNode = InsNode->children[InsStr[i] - 'a'];
	}
		cout << InsStr << endl;
	//	system("pause");
	if (InsNode != T) {
		//解决传入空string的BUG
		//末尾节点标记结尾
		InsNode->str = InsStr;
		InsNode->count++;
		InsNode->terminal = true;
	}

	return OK;

}

int TrieSearch(Trietree *T, string SearchStr)
{
	TrietreeNode SearchMan = T;
	bool tag = true;
	for (unsigned int i = 0; i < SearchStr.length(); i++) {
		if (SearchMan->children[SearchStr[i] - 'a'] == NULL) {
			tag = false;
			break;
		}
		SearchMan = SearchMan->children[SearchStr[i] - 'a'];
	}

	if (tag) {
		if (SearchMan->terminal)
		{
			return SearchMan->count;
		}
	}
	return 0;
}
//创建一个队列
typedef struct Qnode {
	TrietreeNode data;
	struct Qnode * next;
}QNode, *QueuePtr;

typedef struct {
	QueuePtr front;
	QueuePtr rear;
}LinkQueue;

int InitQueue(LinkQueue &Q) {

	Q.front = Q.rear = (QueuePtr)malloc(sizeof(QNode));
	if (!Q.front) exit(OVERFLOW);
	Q.front->next = NULL;
	return OK;
}

int DestroyQueue(LinkQueue & Q) {
	//销毁队列
	while (Q.front) {
		Q.rear = Q.front->next;
		free(Q.front);
		Q.front = Q.rear;
	}
	return OK;
}

int EnQueue(LinkQueue &Q, TrietreeNode node) {
	//插入元素e为Q的新的队尾元素
	QueuePtr p;
	p = (QueuePtr)malloc(sizeof(Qnode));
	if (!p) {
		exit(OVERFLOW);
	}
	p->data = node; p->next = NULL;
	Q.rear->next = p;
	Q.rear = p;
	return OK;
}

int DeQueue(LinkQueue &Q, TrietreeNode &node) {
	if (Q.front == Q.rear) return ERROR;
	QueuePtr p;
	p = Q.front->next;
	node = p->data;
	Q.front->next = p->next;
	if (Q.rear == p) {
		Q.rear = Q.front;
	}
	free(p);
	return OK;
}

bool EmptyQueue(LinkQueue Q) {
	if (Q.front == Q.rear) {
		return true;
	}
	return false;
}

struct maxstr {
	int count;
	string str;
};

void travelTrie(TrietreeNode Tree)
{
	//输出多个文件
	char  temp[10];
	string savefileStr = "savafile";
	_itoa_s(filecount, temp, 2, 10);
	savefileStr = savefileStr + temp + ".txt";
	filecount++;
	cout << savefileStr << endl;
	string str;
	maxstr MaxStr;
	ofstream savefile(savefileStr);
	MaxStr.count = 0;
	MaxStr.str = "";
	LinkQueue Q;
	InitQueue(Q);
	TrietreeNode travelMan = Tree;
	EnQueue(Q, travelMan);
	while (!EmptyQueue(Q)) {
		DeQueue(Q, travelMan);
		if (travelMan->terminal) {
			//			cout << travelMan->str << endl;
		//	界面输出
			//cout << "单词：" << travelMan->str << endl;
			//cout << "出现次数";
			//cout << setw(10) << travelMan->count << " 次" << endl;
			//cout << endl;
			////保存在文件中
			savefile << "单词：" << travelMan->str << endl;
			savefile << "出现次数";
			savefile << setw(10) << travelMan->count << " 次" << endl;
			if (MaxStr.count < travelMan->count) {
				MaxStr.count = travelMan->count;
				MaxStr.str = travelMan->str;
			}
		}
		for (int i = 0; i < ALP_NUM; i++) {
			if (travelMan->children[i] != NULL) {
				EnQueue(Q, travelMan->children[i]);
			}
		}
	}
	cout << "文中出现最多的单词是: " << MaxStr.str << endl;
	cout << "出现次数";
	cout << setw(10) << MaxStr.count << " 次" << endl;
	savefile << "文中出现最多的单词是: " << MaxStr.str << endl;
	savefile << "出现次数";
	savefile << setw(10) << MaxStr.count << " 次" << endl;

	system("pause");

}

void delet() {
	
	remove("workdata.txt");
	for (int num = 0;;num++) {

		char  temp[10];
		string savefileStr = "savafile";
		_itoa_s(num, temp, 2, 10);
		savefileStr = savefileStr + temp + ".txt";
		cout << savefileStr<<endl;
		ifstream showfile(savefileStr);
		if (!showfile.is_open()) {
			break;
		}
		else {
			showfile.close();
		}
		remove(savefileStr.data());
	}
	filecount = 0;
	
}

void showContent(int num) {
	num = num - 1;
	char  temp[10];
	string savefileStr = "savafile";
	_itoa_s(num, temp, 2, 10);
	savefileStr = savefileStr + temp + ".txt";
	ifstream showfile(savefileStr);
	system("cls");
	cout << "正在查看第"<<num+1<<"个文件"<< endl;
	if (!showfile.is_open()) {
		cout << "原文件打开失败！" << endl;
		system("pause");
		return windows();
	}

	if (showfile) {
		while (!showfile.eof()) {
			string outString;
			getline(showfile, outString);
			cout << outString<<endl;
		}
	}
	system("pause");
	
}

string ToLowerString(string str) {
	for (unsigned int i = 0; i < str.length(); i++) {
		str[i] = tolower(str[i]);
	}

	return str;
}

string translate(string str) {
	for (unsigned int i = 0; i < str.length(); i++) {
		if (!isalpha(str[i])) {
			str[i] = ' ';
		}
	}
	str = ToLowerString(str);
	str += " ";
	cout << "正在转换文本.." << endl;
	cout << str << endl;
	return str;
}

void loading(TrietreeNode Tree, string str) {

	ifstream myfile(str);
	ofstream handlefile("workdata.txt");
	ifstream workfile("workdata.txt");
	string temp;
	string firsthandle;
	//文件输入处理
	if (!myfile.is_open()) {
		cout << "原文件打开失败！" << endl;
		system("pause");
		return windows();
	}
	if (myfile) {
		while (!myfile.eof()) {
			cout << "正在转换文本.." << endl;
			system("cls");
			myfile >> firsthandle;
			//			cout << firsthandle << " ";
			firsthandle = translate(firsthandle); //去掉标点符号
			cout << "原字符串转换为：" << firsthandle << endl;
			handlefile << firsthandle;
			system("cls");
		}
		cout << "正在转换文本...." << endl;
		//		cout << endl;
	}
	myfile.close();
	handlefile.close();
	//文件读入
	if (!workfile.is_open()) {
		cout << "处理文件打开失败" << endl;
	}
	if (workfile) {
		system("cls");
		while (!workfile.eof()) {
			workfile >> temp;
			//			temp=ToLowerString(temp);   //单词转换小写
			cout << temp << " ";
			TrieInsert(Tree, temp); //插入字典树
			temp = " ";
		}
	}
	workfile.close();
	cout << "文本加载完成！" << endl;
	system("pause");

}

void windows()
{
	system("cls");
	string str;
	TrietreeNode Tree;
	char ch;
	string searchStr;
	int num;
	Tree = TrieInit();
	cout << "您好,欢迎使用我的字典树！" << endl;
	cout << "请输入要打开的文本:" << endl;
	cin >> str;
	loading(Tree, str);
	travelTrie(Tree);
	system("cls");
	ch = '1';
	while (ch != '0') {
		int SearchCount;
		cout << "您正在处理文件：" << str << endl;
		cout << "1.单词查询" << endl;
		cout << "2.单词统计" << endl;
		cout << "3.继续插入" << endl;
		cout << "0.清除记录" << endl;
		cin >> ch;
		switch (ch)
		{
		case '1':
			system("cls");
			cout << "请输入要搜索的单词" << endl;
			cin >> searchStr;
			SearchCount = TrieSearch(Tree, searchStr);
			if (SearchCount) {
				cout << "单词:" << searchStr << "在文中出现了" << SearchCount << "次" << endl;
			}
			else {
				cout << "单词:" << searchStr << "不存在";
			}
			break;
		case '2':
			cout << "你想查看第几个文件?" << endl;
			cin >> num;
			showContent(num);
			system("cls");
			break;
		case '3': {
			windows();
		}
		case '0':
			delet();
			windows();
			break;
		default:
			cout << "输入错误请重新输入!" << endl;

		}

	}

	travelTrie(Tree);
	system("pause");
}

int main()
{
	hideHandle();
	windows();

	return 0;
}