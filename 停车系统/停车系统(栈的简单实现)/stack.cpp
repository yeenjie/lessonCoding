#include<iostream>
#include<malloc.h>
#include<time.h>
#include<Windows.h>
#define OK 1
#define Parking_Space_NUM 100
#define Add_Space 20
using namespace std;
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
int priStack(Sqstack parking);

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
	if (stack.CarNum < stack.ParkingSpace){
		   //��¼�����ʱ��
		//cout << "������Ľ�stack����" << endl;
		*stack.top = car;
		stack.CarNum++;
		//cout << "��Ľ�����" <<car.CarId<< endl;
		priStack(stack);
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


int CarLeave(Sqstack &mainParking,Sqstack &assistParking,char* leaveId,double &stayTime) {//�����뿪
	InCar theCar;
//	InCar tempCar;
	Stack_pop(mainParking, theCar);             
	while (strcmp(theCar.CarId, leaveId) != 0) {      //�ж��Ƿ���ָ������
		
		Stack_push(assistParking, theCar);			  //���ǾͿ�����������
		//priStack(assistParking);
		//cout << "�ҵ������� ���������������" << /*endl;
		//cout << "��ջ����" <<thecar.carid<< endl;*/
		//system("pause");
		if (mainParking.CarNum > 0) {
			Stack_pop(mainParking, theCar);
		}
		else {
			return ERROR;
		}
	}
	stayTime = (clock()-theCar.ParkingTime)/ CLOCKS_PER_SEC;
	while (assistParking.CarNum) { //���������ĳ�����ȥ
		InCar reCar;
		Stack_pop(assistParking, reCar);
		Stack_push(mainParking, reCar);
	}
	return OK;
	
}

int Carin(Sqstack & mainParking, InCar incar) {
//	cout << "hrea" << endl;
	incar.CarKey = mainParking.CarNum;
//	mainParking.CarNum++;
//	incar.ParkingTime = clock();
	Stack_push(mainParking, incar);
	return OK;
}

int priStack(Sqstack Parking) {
	
	for (int i = 0; i < Parking.CarNum; i++) {
		appGotoXY(10, 10+i);
		cout << Parking.base[i].CarId << endl;
	}
	//system("pause");
	return OK;
}



void operation(Sqstack & mainParking, Sqstack &assistParking) {
	char ch = '1';
	char leaveCarId[20];
	car inCar;
	double time;
	
	while (ch != '0') {
		system("cls");
		priStack(mainParking);
		appGotoXY(30, 10);
		cout << "1.�µĳ���ʻ��" << endl;
		appGotoXY(30, 11);
		cout << "2.����ʻ��" << endl;
		appGotoXY(30, 12);
		cout << "0.�˳�" << endl;
		cin >> ch;
		switch (ch)
		{
		case '1':
			cout << "������ó��ĳ��ƺ�:" << endl;
			cin >> inCar.CarId;
			Carin(mainParking, inCar);
			break;
		case '2':
			cout << "����������ĳ��ƺ�:" << endl;
			cin >> leaveCarId;
			CarLeave(mainParking, assistParking, leaveCarId,time);
			break;
		case'0':
			loading();
		default:
			break;
		}
	}
	
}

void loading() {
	Sqstack mainParking;
	Sqstack assistParking;
	InitStact(mainParking);
	InitStact(assistParking);
	operation(mainParking, assistParking);
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