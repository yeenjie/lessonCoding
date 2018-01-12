#include<iostream>
#include <stdlib.h>
#include <time.h> //用到了time函数 
using namespace std;
int getDgit(int num,int d) {
	
	int val;
	while (d--) {
		val = num % 10;
		num /= 10;
	}
	return val;
}

void RadixSort(int *list, int begin, int end, int digit) {
	int radix = 10;
	int nd=0;
	int * count = new int[radix];
	int * bucket = new int[end - begin + 1];
	for (int d = 1; d <= digit; d++) {
		for (int i = 0; i < radix; i++) {
			count[i] = 0;
		}

		for (int i = begin; i <= end; i++) {
			nd = getDgit(list[i], d);
			count[nd]++;
		}

		for (int i = 1; i < radix; i++) {
			count[i] = count[i] + count[i - 1];
		}

		//排序稳定性
		for (int i = end; i >= begin; i--) {
			nd = getDgit(list[i], d);
			bucket[count[nd] - 1] = list[i];
			count[nd]--;
		}

		for (int i = begin, j = 0; i <= end; j++, i++) {
			list[i] = bucket[j];
		}
	}
}

int main()
{
	srand((unsigned)time(NULL));
	int begin = 0;
	int end = 0;

	int * list = new int[900];
	for (int i = 0; i < 900;i++){
		list[i] = rand() % 900 + 100;
	}
	RadixSort(list, 0, 899, 3);
	for (int i = 0; i < 900; i++) {
		cout << list[i] << endl;
	}
	system("pause");
	return 0;
}