#include "view_handle.h"

#include <Windows.h>
#include <iostream>
#include <conio.h>

using namespace std;

void stdView()
{
	//moi phien lam viec chi duoc thao tac voi 1 truong
	string initSchool;
	initSchool = Schooling();

	int n = 0;
	while(n < 32){
		cout << "\n\n	---MENU---\n";
		cout << " 1-add new student records\n";					//OK
		cout << " 2-show all student records\n";				//OK
		cout << " 3-update a student info\n";					//OK
		cout << " 4-delete a student record\n";					//OK
		cout << " 5-search for records\n";						//OK?
		cout << " 6-do statistics\n";							//OK
		cout << " 0-exit program\n\n";

		int fearture;
		cout << "your choice: ";
		cin >> fearture;

		switch(fearture){
			case 1:
				add(initSchool);
				break;
			case 2:
				show(initSchool);
				break;
			case 3:
				update_show(initSchool);
				break;
			case 4:
				del(initSchool);
				break;
			case 5:
				search(initSchool);
				break;
			case 6:
				stat(initSchool);
				break;
			case 0:
				cout << "\n\twill now exit program..";
				Sleep(3000);
				remove("student_search_result.txt");
				exit(0);
			default:
				cout << "\t!invalid parameter. please choose one of following parameters..\n";
		}
	}
}