#include <iostream>
#include <conio.h>
#include <Windows.h>

#include "student_view.h"
#include "tcp.h"

using namespace std;

CRITICAL_SECTION cs_m, cs_t;

DWORD WINAPI tcpip(LPVOID arg){
	EnterCriticalSection(&cs_t);
	while(1){
		tcp_recv();
		//Sleep(3000);
	}
	LeaveCriticalSection(&cs_t);
	return 0;
}

int main()
{
	cout << "Anh Nguyen Viet - 4th Week Pratices\n";
	cout << "-----------------------------------\n\n";

	InitializeCriticalSection(&cs_t);
	HANDLE hThread2 = CreateThread(NULL, 0, tcpip, NULL, 0, NULL);
	
	stdView();

	WaitForSingleObject(hThread2, INFINITE);
	CloseHandle(hThread2);
	DeleteCriticalSection(&cs_t);

	getch();
	return 0;
}