#include<iostream>
#include<malloc.h>
#include<time.h>
#include <conio.h>
#include<iomanip>
#include <sapi.h>
#include <string>
#include<windows.h>
#include<tchar.h>
#include<stdlib.h>
#pragma comment(lib,"ole32.lib")   //CoInitialize CoCreateInstance��Ҫ����ole32.dll    
#pragma comment(lib,"sapi.lib")    //sapi.lib��SDK��libĿ¼,������ȷ����  
#define OK 1
#define Parking_Space_NUM 50
#define Add_Space 20
#define lineNum 8
using namespace std;

//�����ʶ�
void ToSpeack(string str) {
	char *szStr = (char*)str.c_str();
	ISpVoice *pVoice = nullptr;
	//char * first str1 = "����";
	WCHAR wszClassName[256];
	memset(wszClassName, 0, sizeof(wszClassName));
	MultiByteToWideChar(CP_ACP, 0, szStr, strlen(szStr) + 1, wszClassName,
		sizeof(wszClassName) / sizeof(wszClassName[0]));

	if (FAILED(::CoInitialize(NULL)))
		return;
	//��ȡSpVoice�ӿ�
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
	if (SUCCEEDED(hr))
	{
		pVoice->SetVolume((USHORT)100); //������������Χ�� 0 -100
		pVoice->SetRate(0); //�����ٶȣ���Χ�� -10 - 10
		hr = pVoice->Speak(wszClassName, 0, NULL);
		pVoice->Release();
		pVoice = NULL;
	}
	//�ͷ�com��Դ
	::CoUninitialize();
	return;
}

typedef struct car {
	int CarKey;
	int ParkingTime;
	char CarId[20];
}InCar;

typedef struct {
	InCar *base;
	InCar *top;
	int CarNum;
	int ParkingSpace;
}Sqstack;

void appGotoXY(int x, int y)
{
	CONSOLE_SCREEN_BUFFER_INFO    csbiInfo;
	HANDLE    hConsoleOut;
	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);
	csbiInfo.dwCursorPosition.X = x;
	csbiInfo.dwCursorPosition.Y = y;
	SetConsoleCursorPosition(hConsoleOut, csbiInfo.dwCursorPosition);
}

void loading();
int priStack(Sqstack parking,int page);
void operation(Sqstack & mainParking, Sqstack &assistParking,int page);
int InitStact(Sqstack &stack) {
	stack.base = (InCar*)malloc(Parking_Space_NUM * sizeof(InCar));
	if (!stack.base) {
		exit(OVERFLOW);
	}
	stack.top = stack.base;
	stack.ParkingSpace = Parking_Space_NUM;
	stack.CarNum = 0;
	return OK;
}

int Stack_push(Sqstack &stack, InCar car) {
	if (stack.CarNum < stack.ParkingSpace) {
		//��¼�����ʱ��
		//cout << "������Ľ�stack����" << endl;
		*stack.top = car;
		stack.CarNum++;
		//cout << "��Ľ�����" <<car.CarId<< endl;
		//priStack(stack);
		stack.top++;

	}
	else {

	}
	return OK;
}

int Stack_pop(Sqstack &stack, InCar &getCar) {
	if (stack.CarNum > 0) {
		getCar = *--stack.top;
		stack.CarNum--;
	}
	return OK;
}


int CarLeave(Sqstack &mainParking, Sqstack &assistParking, char* leaveId, InCar& outCar) {//�����뿪
	InCar theCar;
	bool tag = false;
	//	InCar tempCar;

	for (int i = 0; i < mainParking.CarNum; i++) {
		if (strcmp(mainParking.base[i].CarId, leaveId) == 0)
		{
			tag = true;
		}
	}
	if (!tag) {
		return ERROR;
	}
	Stack_pop(mainParking, theCar);
	while (strcmp(theCar.CarId, leaveId) != 0) {      //�ж��Ƿ���ָ������
		Stack_push(assistParking, theCar);

		if (mainParking.CarNum > 0) {
			Stack_pop(mainParking, theCar);
		}
		else {
			return ERROR;
		}												//���ǾͿ�����������
	}											  //priStack(assistParking);
	outCar = theCar;											  //cout << "�ҵ������� ���������������" << /*endl;
												  //cout << "��ջ����" <<thecar.carid<< endl;*/
												  //system("pause");
	while (assistParking.CarNum) { //���������ĳ�����ȥ
		InCar reCar;
		Stack_pop(assistParking, reCar);
		Stack_push(mainParking, reCar);
	}
	priStack(mainParking,1);
	return OK;

}

int Carin(Sqstack & mainParking, InCar incar) {
	//	cout << "hrea" << endl;
	incar.CarKey = mainParking.CarNum;
	//	mainParking.CarNum++;
	incar.ParkingTime = clock();
	Stack_push(mainParking, incar);
	return OK;
}

string gettime(InCar thecar) {
	int seconds=(clock() - thecar.ParkingTime) / CLOCKS_PER_SEC;
	int min;
	int hour;
	char  time[30];
	string timeStr="";
	min = seconds / 60;
	seconds = seconds % 60;
	hour = min / 60;
	min = min % 60;
	_itoa_s(hour, time, 10);
	timeStr = timeStr+time+"Сʱ";
	_itoa_s(min, time, 10);
	timeStr = timeStr + time + "����";
	_itoa_s(seconds, time, 10);
	timeStr = timeStr + time + "��";
	return timeStr;
}

int carheight = 4;
int timeheight = 6;

int showtime(Sqstack Parking,int page) {
	if (page == 1) {
		for (int i = 0; i < Parking.CarNum&&i<24; i++) {

			appGotoXY(15 * (i % (lineNum)), timeheight + (i / lineNum)*6.5);

			cout << "|" << setw(7) << gettime(Parking.base[i]) << "|" << endl;
			cout << " " << endl;
		}
	}
	else if (page == 2) {
		for (int i = 24; i < Parking.CarNum; i++) {//i-24ȷ��24��ʾ��0��λ��

			appGotoXY(15 * ((i-24) % (lineNum)), timeheight + ((i-24) / lineNum)*6.5);

			cout << "|" << setw(7) << gettime(Parking.base[i]) << "|" << endl;
			cout << " " << endl;
		}
	}
	
	return OK;
}

//int showtimenext(Sqstack Parking) {
//	for (int i = 25; i < Parking.CarNum; i++) {
//
//		appGotoXY(15 * (i % (lineNum)), timeheight + (i / lineNum)*6.5);
//
//		cout << "|" << setw(7) << gettime(Parking.base[i]) << setw(7) << "|" << endl;
//		cout << " " << endl;
//	}
//	return OK;
//}

//int pristacknext(sqstack parking) {
//	system("cls");
//	appgotoxy(0, 0);
//	cout << "=======================================================================================================================" << endl;
//	cout << "��ͣ����ʣ�೵λ�� " << parking.parkingspace - parking.carnum << " " << endl;
//	cout << "=======================================================================================================================" << endl;
//
//	for (int i = 25; i < parking.carnum; i++) {
//		appgotoxy(15 * (i % linenum), 3 + 6 * (i / linenum));
//		cout << "  �q�T�T�r" << endl;
//		appgotoxy(15 * (i % linenum), 4 + 6 * (i / linenum));
//		cout << "|" << setw(7) << parking.base[i].carid << setw(7) << "|" << endl;
//		appgotoxy(15 * (i % linenum), 5 + 6 * (i / linenum));
//		cout << setw(7) << " �t�ѨT�Ѩs" << endl;
//		cout << " " << endl;
//
//	}
//	for (int i = 1; i < 3; i++) {
//		appgotoxy(0, 7 * i);
//		cout << "=======================================================================================================================";
//	}
//	appgotoxy(0, 7 * 3);
//	cout << "==========================================================================================================|  ����ͨ��  |";
//	appgotoxy(25, 25);
//	cout << "1.�µĳ���ʻ��" << endl;
//	appgotoxy(48, 25);
//	cout << "2.����ʻ��" << endl;
//	appgotoxy(65, 25);
//	cout << "0.ϵͳ����" << endl;
//
//	//system("pause");
//	return ok;
//}

int priStack(Sqstack Parking,int page) {
	system("cls");
	appGotoXY(0, 0);
	cout << "=======================================================================================================================" << endl;
	cout << "��ͣ����ʣ�೵λ�� " << Parking.ParkingSpace - Parking.CarNum << " " << endl;
	cout << "=======================================================================================================================" << endl;
	if (page == 1) {//ҳ��1����
		for (int i = 0; i < Parking.CarNum&&i<24; i++) { //��һҳֻ��ʾ����24����
			appGotoXY(15 * (i % lineNum), 3 + 6 * (i / lineNum));
			cout << setw(7) << "  �q�T�T�r" << endl;
			appGotoXY(15 * (i % lineNum), 4 + 6 * (i / lineNum));
			cout  << setw(7) << Parking.base[i].CarId <<endl;
			appGotoXY(15 * (i % lineNum), 5 + 6 * (i / lineNum));
			cout << setw(7) << " �t�ѨT�Ѩs" << endl;
			cout << " " << endl;

		}
	}
	else if(page == 2) {//ҳ��2����
						//i-24ȷ���ڶ�ҳ��ʾ24 �� 0��λ��
		for (int i = 24; i < Parking.CarNum; i++) {
			appGotoXY(15 * ((i -24)% lineNum), 3 + 6 * ((i - 24) / lineNum));
			cout << setw(7) << "  �q�T�T�r" << endl;
			appGotoXY(15 * ((i - 24) % lineNum), 4 + 6 * ((i - 24) / lineNum));
			cout << setw(7) << Parking.base[i].CarId  << endl;
			appGotoXY(15 * ((i - 24) % lineNum), 5 + 6 * ((i - 24) / lineNum));
			cout << setw(7) << " �t�ѨT�Ѩs" << endl;
			cout << " " << endl;

		}
	}
	for (int i = 1; i < 3; i++) {
		appGotoXY(0, 7 * i);
		cout << "=======================================================================================================================";
	}
	appGotoXY(0, 7 * 3);
	cout << "==========================================================================================================|  ����ͨ��  |";
	appGotoXY(25, 25);
	cout << "1.�µĳ���ʻ��" << endl;
	appGotoXY(48, 25);
	cout << "2.����ʻ��" << endl;
	appGotoXY(65, 25);
	cout << "0.ϵͳ����" << endl;

	//system("pause");
	return OK;
}

//void operationNext(Sqstack & mainParking, Sqstack &assistParking) {
//	system("cls");
//	char ch = '1';
//	char leaveCarId[20];
//	car inCar;
//	double time;
//	while (ch != '0') {
//
//		priStacknext(mainParking);
//		while (_kbhit() == 0)
//		{
//			showtimenext(mainParking);
//		}
//
//		ch = _getch();
//		appGotoXY(10, 27);
//		switch (ch)
//		{
//		case '1':
//			cout << "������ó��ĳ��ƺ�:";
//			cin >> inCar.CarId;
//			Carin(mainParking, inCar);
//			break;
//		case '2':
//			cout << "����������ĳ��ƺ�:";
//			cin >> leaveCarId;
//			appGotoXY(10, 27);
//			if (CarLeave(mainParking, assistParking, leaveCarId, time)) {
//				cout << "���ƣ�" << leaveCarId << "      " << "ͣ��ʱ��" << time << "��" << endl;
//
//			}
//			else {
//				cout << "�ó������ڣ�              " << endl;
//			}
//
//			system("pause");
//
//			break;
//		case's':
//			appGotoXY(10, 27);
//			cout << "û����һҳ�ˣ�" << endl;
//			break;
//		case'w':
//			return operation(mainParking, assistParking);
//		case'0':
//			loading();
//		default:
//			break;
//		}
//
//	}
//
//}

void operation(Sqstack & mainParking, Sqstack &assistParking,int page) {
		char ch = '1';
		char leaveCarId[20];
		car inCar;
		double time;
		while (ch != '0') {

			priStack(mainParking,page);
			while (_kbhit() == 0)
			{
				showtime(mainParking,page);
			}

			ch = _getch();
			appGotoXY(10, 27);
			switch (ch)
			{
			case '1':
				cout << "������ó��ĳ��ƺ�:";
				cin >> inCar.CarId;
				Carin(mainParking, inCar);
				break;
			case '2':
				InCar OutCar;
				cout << "����������ĳ��ƺ�:";
				cin >> leaveCarId;
				appGotoXY(10, 27);
				if (CarLeave(mainParking, assistParking, leaveCarId, OutCar)) {
					string speakStr = "���ƣ�";
					string timeStr;
					timeStr = gettime(OutCar);
					speakStr = speakStr + leaveCarId+ "     ͣ��ʱ��"+timeStr;
					//cout << speakStr
					cout << "���ƣ�" << leaveCarId << " " << "ͣ��ʱ��:" << timeStr << endl;
					ToSpeack(speakStr);
					

				}
				else {
					cout << "�ó������ڣ�                          " << endl;
				}

				system("pause");

				break;
			case'w':
				if (page == 1) {
					appGotoXY(10, 27);
					cout << "û����һҳ�ˣ�" << endl;
				}
				else if (page == 2) {
					return operation(mainParking, assistParking, 1);
				}
				
				break;
			case's':
				//system("cls");
				if (page == 2) {
					appGotoXY(10, 27);
					cout << "û����һҳ�ˣ�" << endl;
				}
				else if(page == 1) {
					return operation(mainParking, assistParking,2);
				}
				break;
				
			case'0':
				loading();
			default:
				break;
			}

		}

}

int hideHandle() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);//��ȡ�����Ϣ
	cci.bVisible = FALSE;//���ع��
	SetConsoleCursorInfo(hOut, &cci);//���ù����Ϣ
	return 0;
}
void loading() {
	hideHandle();
	Sqstack mainParking;
	Sqstack assistParking;
	InitStact(mainParking);
	InitStact(assistParking);
	string welStr = "��ӭʹ��ͣ������ϵͳ�����ڼ������Ե�...";
	appGotoXY(35, 13);
	cout << "��ӭʹ��ͣ������ϵͳ��"<<endl;
	ToSpeack(welStr);
	operation(mainParking, assistParking,1);
	system("pause");
	return;

}

void menu(Sqstack & mainParking, Sqstack &assistParking) {
	system("cls");



}



int main()
{
	loading();
	return 0;
}