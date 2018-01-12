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
	p = new(Trietree);  //��malloc����ռ�Ļ� string��û�г�ʼ����
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
	GetConsoleCursorInfo(hOut, &cci);//��ȡ�����Ϣ
	cci.bVisible = FALSE;//���ع��
	SetConsoleCursorInfo(hOut, &cci);//���ù����Ϣ
	return 0;
}


int TrieInsert(TrietreeNode & T, string InsStr)
{   //����һ������
	TrietreeNode InsNode = T;
	for (unsigned int i = 0; i < InsStr.length(); i++)
	{
		system("cls");
		if (InsStr[i] == ' ') {
			break;
		}
		cout << "���ڼ�����..." << endl;
	//	cout << "�����ڲ���" << InsStr[i] << endl;
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
		//��������string��BUG
		//ĩβ�ڵ��ǽ�β
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
//����һ������
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
	//���ٶ���
	while (Q.front) {
		Q.rear = Q.front->next;
		free(Q.front);
		Q.front = Q.rear;
	}
	return OK;
}

int EnQueue(LinkQueue &Q, TrietreeNode node) {
	//����Ԫ��eΪQ���µĶ�βԪ��
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
	//�������ļ�
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
		//	�������
			//cout << "���ʣ�" << travelMan->str << endl;
			//cout << "���ִ���";
			//cout << setw(10) << travelMan->count << " ��" << endl;
			//cout << endl;
			////�������ļ���
			savefile << "���ʣ�" << travelMan->str << endl;
			savefile << "���ִ���";
			savefile << setw(10) << travelMan->count << " ��" << endl;
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
	cout << "���г������ĵ�����: " << MaxStr.str << endl;
	cout << "���ִ���";
	cout << setw(10) << MaxStr.count << " ��" << endl;
	savefile << "���г������ĵ�����: " << MaxStr.str << endl;
	savefile << "���ִ���";
	savefile << setw(10) << MaxStr.count << " ��" << endl;

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
	cout << "���ڲ鿴��"<<num+1<<"���ļ�"<< endl;
	if (!showfile.is_open()) {
		cout << "ԭ�ļ���ʧ�ܣ�" << endl;
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
	cout << "����ת���ı�.." << endl;
	cout << str << endl;
	return str;
}

void loading(TrietreeNode Tree, string str) {

	ifstream myfile(str);
	ofstream handlefile("workdata.txt");
	ifstream workfile("workdata.txt");
	string temp;
	string firsthandle;
	//�ļ����봦��
	if (!myfile.is_open()) {
		cout << "ԭ�ļ���ʧ�ܣ�" << endl;
		system("pause");
		return windows();
	}
	if (myfile) {
		while (!myfile.eof()) {
			cout << "����ת���ı�.." << endl;
			system("cls");
			myfile >> firsthandle;
			//			cout << firsthandle << " ";
			firsthandle = translate(firsthandle); //ȥ��������
			cout << "ԭ�ַ���ת��Ϊ��" << firsthandle << endl;
			handlefile << firsthandle;
			system("cls");
		}
		cout << "����ת���ı�...." << endl;
		//		cout << endl;
	}
	myfile.close();
	handlefile.close();
	//�ļ�����
	if (!workfile.is_open()) {
		cout << "�����ļ���ʧ��" << endl;
	}
	if (workfile) {
		system("cls");
		while (!workfile.eof()) {
			workfile >> temp;
			//			temp=ToLowerString(temp);   //����ת��Сд
			cout << temp << " ";
			TrieInsert(Tree, temp); //�����ֵ���
			temp = " ";
		}
	}
	workfile.close();
	cout << "�ı�������ɣ�" << endl;
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
	cout << "����,��ӭʹ���ҵ��ֵ�����" << endl;
	cout << "������Ҫ�򿪵��ı�:" << endl;
	cin >> str;
	loading(Tree, str);
	travelTrie(Tree);
	system("cls");
	ch = '1';
	while (ch != '0') {
		int SearchCount;
		cout << "�����ڴ����ļ���" << str << endl;
		cout << "1.���ʲ�ѯ" << endl;
		cout << "2.����ͳ��" << endl;
		cout << "3.��������" << endl;
		cout << "0.�����¼" << endl;
		cin >> ch;
		switch (ch)
		{
		case '1':
			system("cls");
			cout << "������Ҫ�����ĵ���" << endl;
			cin >> searchStr;
			SearchCount = TrieSearch(Tree, searchStr);
			if (SearchCount) {
				cout << "����:" << searchStr << "�����г�����" << SearchCount << "��" << endl;
			}
			else {
				cout << "����:" << searchStr << "������";
			}
			break;
		case '2':
			cout << "����鿴�ڼ����ļ�?" << endl;
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
			cout << "�����������������!" << endl;

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