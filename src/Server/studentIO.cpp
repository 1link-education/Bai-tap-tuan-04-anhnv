#include "studentIO.h"
#include "sqlite_handle.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <Windows.h>
#include <Winbase.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>

#define MAX_DIRS 25
#define MAX_FILES 255
#define MAX_BUFFER 4096

using namespace std;

extern "C" {
	WINBASEAPI BOOL WINAPI
		ReadDirectoryChangesW( HANDLE hDirectory,
		LPVOID lpBuffer, DWORD nBufferLength,
		BOOL bWatchSubtree, DWORD dwNotifyFilter,
		LPDWORD lpBytesReturned,
		LPOVERLAPPED lpOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
		);
}

// chua thong tin thu muc, cung cap bo dem chua thong tin thay doi file
typedef struct _DIRECTORY_INFO {
	HANDLE hDir;
	TCHAR lpszDirName[MAX_PATH];
	CHAR lpBuffer[MAX_BUFFER];
	DWORD dwBufLength;
	OVERLAPPED Overlapped;
} DIRECTORY_INFO, *PDIRECTORY_INFO, *LPDIRECTORY_INFO;

DIRECTORY_INFO DirInfo[MAX_DIRS];						// buffer cho tat ca thu muc
TCHAR FileList[MAX_FILES*MAX_PATH];						// buffer cho tat ca file
DWORD numDirs;

// call on a separate thread to avoid blocking the main thread.  
void WatchDirectory() {
	TCHAR path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, path);
	char buf[2048];
	DWORD nRet;
	BOOL result = TRUE;
	char filename[MAX_PATH];
	DirInfo[0].hDir = CreateFile (path, GENERIC_READ|FILE_LIST_DIRECTORY, 
		FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
		NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OVERLAPPED,
		NULL);

	if(DirInfo[0].hDir == INVALID_HANDLE_VALUE) {
		cout << "\n\t!dir not found :-)\n";
	}

	lstrcpy( DirInfo[0].lpszDirName, path);
	OVERLAPPED PollingOverlap;

	FILE_NOTIFY_INFORMATION* pNotify;
	int offset;
	PollingOverlap.OffsetHigh = 0;
	PollingOverlap.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	while(result) {
		result = ReadDirectoryChangesW(
			DirInfo[0].hDir,											// handle to the directory to be watched
			&buf,														// pointer to the buffer to receive the read results
			sizeof(buf),												// length of lpBuffer
			TRUE,														// flag for monitoring directory or directory tree
			FILE_NOTIFY_CHANGE_FILE_NAME |
			FILE_NOTIFY_CHANGE_DIR_NAME |
			FILE_NOTIFY_CHANGE_SIZE,
			&nRet,														// number of bytes returned
			&PollingOverlap,											// pointer to structure needed for overlapped I/O
			NULL);

		WaitForSingleObject(PollingOverlap.hEvent,INFINITE);
		offset = 0;
		size_t ext;
		string rawName;
		string filenameToString;


		do {
			pNotify = (FILE_NOTIFY_INFORMATION*)((char*)buf + offset);
			strcpy(filename, "");
			int filenamelen = WideCharToMultiByte(CP_ACP, 0, pNotify->FileName, pNotify->FileNameLength/2, filename, sizeof(filename), NULL, NULL);
			filename[pNotify->FileNameLength/2] = '\0';
			switch(pNotify->Action) {
				case FILE_ACTION_ADDED:
					if(strcmp(filename, "DIEM_THI_2016.db") != 0 && (strcmp(filename, "DIEM_THI_2016.db-journal") != 0) && (strcmp(filename, "studentIN.txt") != 0))
					{
						cout << "\n\tthe file is added to the directory: '" << filename << "'\n";
						filenameToString = string(filename);		//OK!
					
						ext = filenameToString.find_first_of("-");					
						if(ext == std::string::npos) {				
							return;
						}
						else {
							//cout << "//found '-' at: " << ext << endl;				//test
							rawName = filenameToString.substr(0, ext);
							//cout << "//rawName: " << rawName << endl;
							importFile(rawName, filename);
							Sleep(1000);
						}
					}
					else
						return;

					break;

				//case FILE_ACTION_MODIFIED:
				//	if((strcmp(filename, "DIEM_THI_2016.db") != 0) && (strcmp(filename, "DIEM_THI_2016.db-journal") != 0) && (strcmp(filename, "studentIN.txt") != 0))
				//	{
				//		printf("\n\tthe file is modified.\n\tthis can be a change in the time stamp or attributes: [%s]\n", filename);
				//		filenameToString = string(filename);
				//		ext = filenameToString.find_first_of("-");//Sleep(10000);
				//		if(ext == std::string::npos) {
				//			return;
				//		}
				//		else {
				//			cout << "//found '-' at: " << ext << endl;				//test
				//			rawName = filenameToString.substr(0, ext);
				//			cout << "//rawName: " << rawName << endl;
				//			importFile(rawName, filename);
				//		}
				//	}
				//	else
				//		return;

				//	break;
				default:
					break;
			}
			offset += pNotify->NextEntryOffset;
		} while(pNotify->NextEntryOffset);												//(offset != 0);
	}

	CloseHandle( DirInfo[0].hDir );
}

void importFile(string tbl_name, char file_name[MAX_PATH])
{
	ifstream inFile;
	string m, p, c;

	cout << "\n\timporting records from files to database...\n";
	Sleep(2000);
	inFile.open(file_name, ios::in);

	while (!inFile.eof())
	{
		Student st;
		getline(inFile, st.id);
		getline(inFile, st.name);
		getline(inFile, st.address);
		getline(inFile, st.sex);
		getline(inFile, m);			
		st.math = atof(m.c_str());
		getline(inFile, p);
		st.phys = atof(p.c_str());
		getline(inFile, c);
		st.chem = atof(c.c_str());
		sql_insert(tbl_name, st);
	}
	string last_row_id = "";
	sql_delete(tbl_name, last_row_id);				// haha! there was a BUG :v

	inFile.close();
	cout << "\n\tstudent records have successfully imported";
	cout << "\n\tfrom [" << file_name << "] to table [" << tbl_name << "]\n";
}