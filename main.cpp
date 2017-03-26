#include <iostream>
#include "windows.h"

using std::cout;
using std::endl;

const int n = 4;
const int p = 4;
const int h = n / p;

int MO[n][n];
int MK[n][n];
int T[n];
int Z[n];
int e;
int d;
int a = INT_MIN;

HANDLE S1 = CreateSemaphore(NULL, 0, 3, NULL);
HANDLE S2 = CreateSemaphore(NULL, 0, 3, NULL);
HANDLE S3 = CreateSemaphore(NULL, 0, 3, NULL);
HANDLE S4 = CreateSemaphore(NULL, 0, 3, NULL);

HANDLE M1 = CreateMutex(NULL, FALSE, NULL);

CRITICAL_SECTION section;

HANDLE E1in = CreateEvent(NULL, true, false, NULL);
HANDLE E3in = CreateEvent(NULL, true, false, NULL);
HANDLE E4in = CreateEvent(NULL, true, false, NULL);

HANDLE E2out = CreateEvent(NULL, false, false, NULL);
HANDLE E3out = CreateEvent(NULL, false, false, NULL);
HANDLE E4out = CreateEvent(NULL, false, false, NULL);

HANDLE inputEvents[3] = { E1in, E3in, E4in };
HANDLE outputEvents[3] = { E2out, E3out, E4out };

void inputMatrix(int matrix[n][n]) {
	for (int i = 0; i <n; ++i) {
		for (int j = 0; j <n; ++j) {
			matrix[i][j] = 1;
		}
	}
}

void inputVector(int vector[n]) {
	for (int i = 0; i <n; ++i) {
		vector[i] = 1;
	}
}

void mulMatrXMatr(int M1[n][n], int M2[n][n], int MOUT[n][n], int begin, int end) {
	int m[n][n];
	for (int i = begin; i <end; ++i) {
		for (int j = 0; j <n; ++j) {
			m[i][j] = 0;
			for (int k = 0; k <n; ++k) {
				m[i][j] = m[i][j] + M1[i][k] * M2[k][j];
			}
		}
		for (int l = 0; l <n; ++l) {
			MOUT[i][l] = m[i][l];
		}
	}
}

void mulMatrixXVector(int M1[n][n], int V1[n], int VOUT[n], int begin, int end) {
	for (int i = begin; i < end; ++i) {
		VOUT[i] = 0;
		for (int j = 0; j < n; ++j) {
			VOUT[i] += M1[i][j] * V1[j];
		}
	}
}

void mulVectorXNumber(int z, int V[n], int begin, int end) {
	for (int i = begin; i < end; ++i) {
		V[i] = V[i] * z;
	}
}

void add(int V1[n], int V2[n], int begin, int end) {
	for (int i = 0; i < end; ++i) {
		V1[i] = V1[i] + V2[i];
	}
}

int findMax(int V[n], int begin, int end) {
	int a = INT_MIN;
	for (int i = begin; i <end; ++i) {
		if (a<V[i]) {
			a = V[i];
		}
	}
	return a;
}

int function(int Z[n], int T[n], int MO[n][n], int MK[n][n], int e, int d, int begin, int end) {
	int V[n];
	int MOUT[n][n];
	mulMatrXMatr(MK, MO, MOUT, begin, end);
	mulMatrixXVector(MOUT, T, V, begin, end);
	mulVectorXNumber(d, V, begin, end);
	mulVectorXNumber(e, Z, begin, end);
	add(V, Z, begin, end);

	return findMax(V, begin, end);
}

void task1(){
	cout << "Task 1 started" << endl;

	//input
	e = 1;
	inputVector(Z);
	//sending a signal to T2,T3,T4
	SetEvent(E1in);
	//    waiting for signal from T3,T4
	WaitForMultipleObjects(3, inputEvents, true, INFINITE);

	int e1;
	int d1;
	int T1[n];
	int MO1[n][n];
	int buff1;
	//copy e, d, T, MO
	EnterCriticalSection(&section);
	e1 = e;
	d1 = d;
	memcpy(T1, T, sizeof(T));
	memcpy(MO1, MO, sizeof(MO));
	LeaveCriticalSection(&section);
	//  account
	buff1 = function(Z, T1, MO1, MK, e1, d1, 0, h);

	WaitForSingleObject(M1, INFINITE);
	if (a<buff1) a = buff1;
	ReleaseMutex(M1);

	//  sending a signal to T2,T3,T4
	ReleaseSemaphore(S1, 3, NULL);
	//    waiting for signal from T2,T3,T4
	WaitForSingleObject(S2, INFINITE);
	WaitForSingleObject(S3, INFINITE);
	WaitForSingleObject(S4, INFINITE);

	//output
	printf("%d", a);

	cout << "Task 1 finished" << endl;

}

void task2() {
	cout << "Task 2 started" << endl;

	//    waiting for signal from T1, T3,T4
	WaitForMultipleObjects(3, inputEvents, true, INFINITE);

	int e2;
	int d2;
	int T2[n];
	int MO2[n][n];
	int buff2;
	//copy e, d, T, MO
	EnterCriticalSection(&section);
	e2 = e;
	d2 = d;
	memcpy(T2, T, sizeof(T));
	memcpy(MO2, MO, sizeof(MO));
	LeaveCriticalSection(&section);
	//  account
	buff2 = function(Z, T2, MO2, MK, e2, d2, 0, h);

	WaitForSingleObject(M1, INFINITE);
	if (a<buff2) a = buff2;
	ReleaseMutex(M1);

	//  sending a signal to T2,T3,T4
	ReleaseSemaphore(S2, 3, NULL);
	//    waiting for signal from T2,T3,T4
	WaitForSingleObject(S1, INFINITE);
	WaitForSingleObject(S3, INFINITE);
	WaitForSingleObject(S4, INFINITE);

	SetEvent(E2out);
	cout << "Task 2 finished" << endl;
}

void task3() {
	cout << "Task 3 started" << endl;
	// input
	d = 1;
	inputVector(T);
	inputMatrix(MO);

	//    sending a signal to T1,T2,T4
	SetEvent(E3in);

	//    waiting for signal from T1,T4
	WaitForMultipleObjects(3, inputEvents, true, INFINITE);

	int e3;
	int d3;
	int T3[n];
	int MO3[n][n];
	int buff3;
	//copy e, d, T, MO
	EnterCriticalSection(&section);
	e3 = e;
	d3 = d;
	memcpy(T3, T, sizeof(T));
	memcpy(MO3, MO, sizeof(MO));
	LeaveCriticalSection(&section);
	//  account
	buff3 = function(Z, T3, MO3, MK, e3, d3, 0, h);

	WaitForSingleObject(M1, INFINITE);
	if (a<buff3) a = buff3;
	ReleaseMutex(M1);

	//  sending a signal to T2,T3,T4
	ReleaseSemaphore(S3, 3, NULL);
	//    waiting for signal from T2,T3,T4
	WaitForSingleObject(S1, INFINITE);
	WaitForSingleObject(S2, INFINITE);
	WaitForSingleObject(S4, INFINITE);

	SetEvent(E3out);

	cout << "Task 3 finished" << endl;
}

void task4() {
	cout << "Task 4 started" << endl;
	// input
	inputMatrix(MK);

	//    sending a signal to T1,T2,T3
	SetEvent(E4in);

	//    waiting for signal from T1,T4
	WaitForMultipleObjects(3, inputEvents, true, INFINITE);

	int e4;
	int d4;
	int T4[n];
	int MO4[n][n];
	int buff4;
	//copy e, d, T, MO
	EnterCriticalSection(&section);
	e4 = e;
	d4 = d;
	memcpy(T4, T, sizeof(T));
	memcpy(MO4, MO, sizeof(MO));
	LeaveCriticalSection(&section);
	//  account
	buff4 = function(Z, T4, MO4, MK, e4, d4, 0, h);

	WaitForSingleObject(M1, INFINITE);
	if (a<buff4) a = buff4;
	ReleaseMutex(M1);

	//  sending a signal to T2,T3,T4
	ReleaseSemaphore(S4, 3, NULL);
	//    waiting for signal from T2,T3,T4
	WaitForSingleObject(S1, INFINITE);
	WaitForSingleObject(S2, INFINITE);
	WaitForSingleObject(S3, INFINITE);

	SetEvent(E4out);

	cout << "Task 4 finished" << endl;
}

int main() {
	cout << "Program started" << endl;

	HANDLE T1 = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)task1, NULL, CREATE_SUSPENDED, NULL);
	HANDLE T2 = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)task2, NULL, CREATE_SUSPENDED, NULL);
	HANDLE T3 = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)task3, NULL, CREATE_SUSPENDED, NULL);
	HANDLE T4 = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)task4, NULL, CREATE_SUSPENDED, NULL);

	InitializeCriticalSection(&section);

	ResumeThread(T2);
	ResumeThread(T3);
	ResumeThread(T4);
	ResumeThread(T1);

	WaitForSingleObject(T1, INFINITE);
	WaitForSingleObject(T2, INFINITE);
	WaitForSingleObject(T3, INFINITE);
	WaitForSingleObject(T4, INFINITE);

	CloseHandle(T1);
	CloseHandle(T2);
	CloseHandle(T3);
	CloseHandle(T4);

	cout << "Program finished" << endl;
	system("pause");

	return 0;
}
