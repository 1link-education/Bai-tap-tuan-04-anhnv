#include "listener.h"
#include "studentIO.h"
#include "sqlite_handle.h"

#include <Windows.h>
#include <iostream>
#include <conio.h>


using namespace std;

CRITICAL_SECTION cs_m, cs_tr;

DWORD WINAPI monitor(LPVOID arg){
	EnterCriticalSection(&cs_m);
	while(1){
		WatchDirectory();
		Sleep(3000);
	}
	LeaveCriticalSection(&cs_m);
	return 0;
}

DWORD WINAPI tcpip_recv(LPVOID arg){
	EnterCriticalSection(&cs_tr);
	while(1){
		tcp_recv();
		//Sleep(3000);
	}
	LeaveCriticalSection(&cs_tr);
	return 0;
}

using namespace std;

int main()
{
	sql_init_db();
	cout << "Anh Nguyen Viet - Server\n";
	cout << "------------------------\n\n";

	InitializeCriticalSection(&cs_m);
	InitializeCriticalSection(&cs_tr);
	HANDLE hThread = CreateThread(NULL, 0, monitor, NULL, 0, NULL);
	HANDLE hThread2 = CreateThread(NULL, 0, tcpip_recv, NULL, 0, NULL);

	WaitForSingleObject(hThread, INFINITE);
	WaitForSingleObject(hThread2, INFINITE);
	CloseHandle(hThread);
	CloseHandle(hThread2);
	DeleteCriticalSection(&cs_m);
	DeleteCriticalSection(&cs_tr);

	getch();
	return 0;
}