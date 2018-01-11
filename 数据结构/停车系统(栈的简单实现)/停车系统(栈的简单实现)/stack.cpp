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
#pragma comment(lib,"ole32.lib")   //CoInitialize CoCreateInstance需要调用ole32.dll    
#pragma comment(lib,"sapi.lib")    //sapi.lib在SDK的lib目录,必需正确配置  
#define OK 1
#define Parking_Space_NUM 50
#define Add_Space 20
#define lineNum 8
using namespace std;

//语音朗读
void ToSpeack(string str) {
	char *szStr = (char*)str.c_str();
	ISpVoice *pVoice = nullptr;
	//char * first str1 = "测试";
	WCHAR wszClassName[256];
	memset(wszClassName, 0, sizeof(wszClassName));
	MultiByteToWideChar(CP_ACP, 0, szStr, strlen(szStr) + 1, wszClassName,
		sizeof(wszClassName) / sizeof(wszClassName[0]));

	if (FAILED(::CoInitialize(NULL)))
		return;
	//获取SpVoice接口
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
	if (SUCCEEDED(hr))
	{
		pVoice->SetVolume((USHORT)100); //设置音量，范围是 0 -100
		pVoice->SetRate(0); //设置速度，范围是 -10 - 10
		hr = pVoice->Speak(wszClassName, 0, NULL);
		pVoice->Release();
		pVoice = NULL;
	}
	//释放com资源
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
		//记录进入的时间
		//cout << "看看真的进stack了吗" << endl;
		*stack.top = car;
		stack.CarNum++;
		//cout << "真的进来了" <<car.CarId<< endl;
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


int CarLeave(Sqstack &mainParking, Sqstack &assistParking, char* leaveId, InCar& outCar) {//车辆离开
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
	while (strcmp(theCar.CarId, leaveId) != 0) {      //判断是否是指定车辆
		Stack_push(assistParking, theCar);

		if (mainParking.CarNum > 0) {
			Stack_pop(mainParking, theCar);
		}
		else {
			return ERROR;
		}												//不是就开到辅助车场
	}											  //priStack(assistParking);
	outCar = theCar;											  //cout << "我到这里了 看看问题出在哪里" << /*endl;
												  //cout << "进栈的是" <<thecar.carid<< endl;*/
												  //system("pause");
	while (assistParking.CarNum) { //辅助车场的车开回去
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
	timeStr = timeStr+time+"小时";
	_itoa_s(min, time, 10);
	timeStr = timeStr + time + "分钟";
	_itoa_s(seconds, time, 10);
	timeStr = timeStr + time + "秒";
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
		for (int i = 24; i < Parking.CarNum; i++) {//i-24确保24显示在0的位置

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
//	cout << "主停车场剩余车位： " << parking.parkingspace - parking.carnum << " " << endl;
//	cout << "=======================================================================================================================" << endl;
//
//	for (int i = 25; i < parking.carnum; i++) {
//		appgotoxy(15 * (i % linenum), 3 + 6 * (i / linenum));
//		cout << "  ╭══╮" << endl;
//		appgotoxy(15 * (i % linenum), 4 + 6 * (i / linenum));
//		cout << "|" << setw(7) << parking.base[i].carid << setw(7) << "|" << endl;
//		appgotoxy(15 * (i % linenum), 5 + 6 * (i / linenum));
//		cout << setw(7) << " ╰⊙═⊙╯" << endl;
//		cout << " " << endl;
//
//	}
//	for (int i = 1; i < 3; i++) {
//		appgotoxy(0, 7 * i);
//		cout << "=======================================================================================================================";
//	}
//	appgotoxy(0, 7 * 3);
//	cout << "==========================================================================================================|  进出通道  |";
//	appgotoxy(25, 25);
//	cout << "1.新的车辆驶入" << endl;
//	appgotoxy(48, 25);
//	cout << "2.车辆驶出" << endl;
//	appgotoxy(65, 25);
//	cout << "0.系统重载" << endl;
//
//	//system("pause");
//	return ok;
//}

int priStack(Sqstack Parking,int page) {
	system("cls");
	appGotoXY(0, 0);
	cout << "=======================================================================================================================" << endl;
	cout << "主停车场剩余车位： " << Parking.ParkingSpace - Parking.CarNum << " " << endl;
	cout << "=======================================================================================================================" << endl;
	if (page == 1) {//页面1操作
		for (int i = 0; i < Parking.CarNum&&i<24; i++) { //第一页只显示少于24辆车
			appGotoXY(15 * (i % lineNum), 3 + 6 * (i / lineNum));
			cout << setw(7) << "  ╭══╮" << endl;
			appGotoXY(15 * (i % lineNum), 4 + 6 * (i / lineNum));
			cout  << setw(7) << Parking.base[i].CarId <<endl;
			appGotoXY(15 * (i % lineNum), 5 + 6 * (i / lineNum));
			cout << setw(7) << " ╰⊙═⊙╯" << endl;
			cout << " " << endl;

		}
	}
	else if(page == 2) {//页面2操作
						//i-24确保第二页显示24 在 0的位置
		for (int i = 24; i < Parking.CarNum; i++) {
			appGotoXY(15 * ((i -24)% lineNum), 3 + 6 * ((i - 24) / lineNum));
			cout << setw(7) << "  ╭══╮" << endl;
			appGotoXY(15 * ((i - 24) % lineNum), 4 + 6 * ((i - 24) / lineNum));
			cout << setw(7) << Parking.base[i].CarId  << endl;
			appGotoXY(15 * ((i - 24) % lineNum), 5 + 6 * ((i - 24) / lineNum));
			cout << setw(7) << " ╰⊙═⊙╯" << endl;
			cout << " " << endl;

		}
	}
	for (int i = 1; i < 3; i++) {
		appGotoXY(0, 7 * i);
		cout << "=======================================================================================================================";
	}
	appGotoXY(0, 7 * 3);
	cout << "==========================================================================================================|  进出通道  |";
	appGotoXY(25, 25);
	cout << "1.新的车辆驶入" << endl;
	appGotoXY(48, 25);
	cout << "2.车辆驶出" << endl;
	appGotoXY(65, 25);
	cout << "0.系统重载" << endl;

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
//			cout << "请输入该车的车牌号:";
//			cin >> inCar.CarId;
//			Carin(mainParking, inCar);
//			break;
//		case '2':
//			cout << "请输入出车的车牌号:";
//			cin >> leaveCarId;
//			appGotoXY(10, 27);
//			if (CarLeave(mainParking, assistParking, leaveCarId, time)) {
//				cout << "车牌：" << leaveCarId << "      " << "停车时间" << time << "秒" << endl;
//
//			}
//			else {
//				cout << "该车不存在！              " << endl;
//			}
//
//			system("pause");
//
//			break;
//		case's':
//			appGotoXY(10, 27);
//			cout << "没有下一页了！" << endl;
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
				cout << "请输入该车的车牌号:";
				cin >> inCar.CarId;
				Carin(mainParking, inCar);
				break;
			case '2':
				InCar OutCar;
				cout << "请输入出车的车牌号:";
				cin >> leaveCarId;
				appGotoXY(10, 27);
				if (CarLeave(mainParking, assistParking, leaveCarId, OutCar)) {
					string speakStr = "车牌：";
					string timeStr;
					timeStr = gettime(OutCar);
					speakStr = speakStr + leaveCarId+ "     停车时间"+timeStr;
					//cout << speakStr
					cout << "车牌：" << leaveCarId << " " << "停车时间:" << timeStr << endl;
					ToSpeack(speakStr);
					

				}
				else {
					cout << "该车不存在！                          " << endl;
				}

				system("pause");

				break;
			case'w':
				if (page == 1) {
					appGotoXY(10, 27);
					cout << "没有上一页了！" << endl;
				}
				else if (page == 2) {
					return operation(mainParking, assistParking, 1);
				}
				
				break;
			case's':
				//system("cls");
				if (page == 2) {
					appGotoXY(10, 27);
					cout << "没有下一页了！" << endl;
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
	GetConsoleCursorInfo(hOut, &cci);//获取光标信息
	cci.bVisible = FALSE;//隐藏光标
	SetConsoleCursorInfo(hOut, &cci);//设置光标信息
	return 0;
}
void loading() {
	hideHandle();
	Sqstack mainParking;
	Sqstack assistParking;
	InitStact(mainParking);
	InitStact(assistParking);
	string welStr = "欢迎使用停车管理系统！正在加载请稍等...";
	appGotoXY(35, 13);
	cout << "欢迎使用停车管理系统！"<<endl;
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