#include <omp.h>
#include <iostream>
#include "windows.h"
#define n 2000
#define p 4

// Assignment #3
// Student: Smishniy Dmitriy
// Group: IO-41
// Date: 07/04/2017
// a=max(e*Z+d*T*(MO*MK))

using namespace std;

int MO[n][n], MK[n][n];
int T[n], Z[n];
int e, d;
int a = INT_MIN;

int H = n / 4;

int main(int argc, char *argv[]) {
	int eLocal = 0;
	int dLocal = 0;
	int MOLocal[n][n];
	int TLocal[n];
	int aLocal;
	int buffMatr[n][n];
	int buffV[n];
	printf("Program started now\n");

	#pragma comment(linker, "/STACK:200000000")
	omp_set_num_threads(4);
	#pragma omp parallel 
	{
		int thread_id = omp_get_thread_num();
		printf("Thread %d started!\n", thread_id+1);
		switch (thread_id)
		{
		case 0: 
			for (int i = 0; i <n; ++i) {
				Z[i] = 1;
			}
			e = 1;
			break;
		case 1: 
			break;
		case 2: 
			d = 1;
			for (int i = 0; i <n; ++i) {
				T[i] = 1;
			}
			for (int i = 0; i <n; ++i) {
				for (int j = 0; j <n; ++j) {
					MO[i][j] = 1;
				}
			}
			break;
		case 3: 
			for (int i = 0; i <n; ++i) {
				for (int j = 0; j <n; ++j) {
					MK[i][j] = 1;
				}
			}
			break;
		}
		
	#pragma omp barrier 
			#pragma omp critical (cs)
			{
				memcpy(MOLocal, MO, sizeof(MOLocal));
				memcpy(TLocal, T, sizeof(TLocal));
				eLocal = e;
				dLocal = d;
			}

			int start = thread_id * H;
			int end = start + H;
			for (int i = start; i < end; i++) {
				buffV[i] = 0;
				for (int j = 0; j < n; j++) {
					buffMatr[i][j] = 0;
					for (int k = 0; k < n; k++) {
						buffMatr[i][j] = buffMatr[i][j] + MO[i][k] * MK[k][j];
					}
					buffV[i] += buffMatr[i][j] * TLocal[j];
				}
				buffV[i] *= dLocal;
				buffV[i] += Z[i] * eLocal;
			}

			for (int i = start; i <end; ++i) {
				if (aLocal<buffV[i]) {
					aLocal = buffV[i];
				}
			}

			#pragma omp critical (csA)
			{
				if (a < aLocal) a = aLocal;
			}

			int thread_number = thread_id + 1;
			printf("Thread finished: %d\n", thread_number);
	#pragma omp barrier
		if (thread_id == 0)
			printf("%d\n", a);
	}
	printf("Program finished now\n");
	system("pause");

	return 0;
}