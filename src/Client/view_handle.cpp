#include "tcp.h"
#include "student.h"

#include <iostream>
#include <string>
#include <Windows.h>
#include <conio.h>
#include <iomanip>
#include <fstream>
#include <stdlib.h>
#include <sstream>

#define MAXSTD 32
#define MAXPATH 255

using namespace std;
using std::string;


string Schooling()
{
	string inSchool;
	cout << "\nenter school name to continue: ";
	fflush(stdin);
	getline(cin, inSchool);
	cin.ignore();
	while(!checkString(inSchool)) {
		cout << "\t!'" << inSchool << "' is not a valid school name. please re-enter school name\n";
		cout << "\nschool name: ";
		fflush(stdin);
		getline(cin, inSchool); 
		cin.ignore();
	}

	string schl_to_str;
	schl_to_str += inSchool;
	char * schl_to_send = &schl_to_str[0];
	tcp_send(schl_to_send);
	return inSchool;
}

// them ban ghi tu ban phim
void addFK(string schoolName)
{
	Student st[MAXSTD];
	int n;
	cout << "\nnumber of records to add: ";
	cin >> n;
	cin.ignore();
	int i;
	for(i = 1; i <= n; i++){
		cout << "\nstudent number " << i << "\n----------------\n\n";
		st[i].importStd();

		string std_to_str = "aFK:";
		std_to_str += schoolName;
		std_to_str += ":";
		stringstream stream;
		
		string math_str;
		stream << st[i].getMath();
		math_str = stream.str();
		stream.str(string());								// to clear string stream buffer
		stream.clear();

		string phys_str;
		stream << st[i].getPhys();
		phys_str = stream.str();
		stream.str(string());
		stream.clear();

		string chem_str;
		stream << st[i].getChem();
		chem_str = stream.str();
		stream.str(string());
		stream.clear();

		string totl_str;
		stream << st[i].getTotal();
		totl_str = stream.str();
		stream.str(string());
		stream.clear();

		std_to_str += st[i].getID();
		std_to_str += ":";
		std_to_str += st[i].getName();
		std_to_str += ":";
		std_to_str += st[i].getAdd();
		std_to_str += ":";
		std_to_str += st[i].getSex();
		std_to_str += ":";
		std_to_str += math_str;
		std_to_str += ":";
		std_to_str += phys_str;
		std_to_str += ":";
		std_to_str += chem_str;
		std_to_str += ":";
		std_to_str += totl_str;

		char * std_to_send = &std_to_str[0];
		tcp_send(std_to_send);
	}
	cout << "\n\nstudent record has been created successfully\n";
}

// them ban ghi tu file studentIN.txt
void addFF(string schoolName)
{
	cout << "\n\timporting records from files to database...\n";

	char * add_signal;
	string add_signal_str = "aFF:";
	add_signal_str += schoolName;
	add_signal = &add_signal_str[0];
	tcp_send(add_signal);

	cout << "\n\tstudent records have successfully imported";
	cout << "\n\tfrom studentIN.txt to table [" << schoolName << "]\n";
}

// showAdd()
void add(string schoolName)
{
	char addOpt;

	cout << "\tpress 'k' to add records from keyboard or 'f' to add from file: ";
	cin >> addOpt;
	cin.ignore();

	switch(addOpt) {
		case 'k':
			addFK(schoolName);
			break;
		case 'f':
			addFF(schoolName);
			break;
		default:
			cout << "\n\t!invalid parameter. please choose [k/f] to add record from keyboard/file..\n";
	}

	cout << "\tpress any key to continue..\n";
	cin.get();
}
 // hien thi toan bo cac ban ghi cua truong dang thao tac
void show(string schoolName)
{
	
	Student st;
	
	cout << "\n\n\t\t---show all records---\n\n";
	
	string show_signal_str = "shw:";
	show_signal_str += schoolName;
	char * show_signal = &show_signal_str[0];
	tcp_send(show_signal);

	cin.get();
}

void updating(string schoolName, string id, string argv)
{
	string name_inp;
	string math_inp;
	string phys_inp;
	string chem_inp;
	int i;

	for(i = 0; i < argv.size(); i++)
	{
		if(argv[i] == 'n') {
				cout << "\nnew name to set: ";
				fflush(stdin);
				getline(cin, name_inp);
		}
		if(argv[i] == 'm') {
				cout << "\nnew math point to set: ";
				fflush(stdin);
				getline(cin, math_inp);
				float math;
				while ((!ValidateFloat(math_inp))) {
					cin.clear();
					cout << "	!invalid number: " << math_inp << endl;
					math_inp.clear();
					cout << "math point: ";
					fflush(stdin);
					getline(cin, math_inp);
				}
				math = atof(math_inp.c_str());
				cin.clear();

				while(!(0 <= math && math <= 10)) {
					cout << "	!a point value must be between 0 and 10. please re-enter math point\n";
					math_inp.clear();
					cout << "math point: ";
					fflush(stdin);
					getline(cin, math_inp);

					while ((!ValidateFloat(math_inp))) {
						cin.clear();
						cout << "	!invalid number: " << math_inp << endl;
						math_inp.clear();
						cout << "math point: ";
						fflush(stdin);
						getline(cin, math_inp);
					}
					math = atof(math_inp.c_str());
					cin.clear();
				}
		}
		if(argv[i] == 'p') {
				cout << "\nnew physics point to set: ";
				fflush(stdin);
				getline(cin, phys_inp);
				float phys;
				while ((!ValidateFloat(phys_inp))) {
					cin.clear();
					cout << "	!invalid number: " << phys_inp << endl;
					phys_inp.clear();
					cout << "physics point: ";
					fflush(stdin);
					getline(cin, phys_inp);
				}
				phys = atof(phys_inp.c_str());
				cin.clear();

				while(!(0 <= phys && phys <= 10)) {
					cout << "	!a point value must be between 0 and 10. please re-enter physics point\n";
					phys_inp.clear();
					cout << "physics point: ";
					fflush(stdin);
					getline(cin, phys_inp);

					while ((!ValidateFloat(phys_inp))) {
						cin.clear();
						cout << "	!invalid number: " << phys_inp << endl;
						phys_inp.clear();
						cout << "physics point: ";
						fflush(stdin);
						getline(cin, phys_inp);
					}
					phys = atof(phys_inp.c_str());
					cin.clear();
				}
		}
		if(argv[i] == 'c') {
				cout << "\nnew chemistry point to set: ";
				fflush(stdin);
				getline(cin, chem_inp);
				float chem;
				while ((!ValidateFloat(chem_inp))) {
					cin.clear();
					cout << "	!invalid number: " << chem_inp << endl;
					chem_inp.clear();
					cout << "chemistry point: ";
					fflush(stdin);
					getline(cin, chem_inp);
				}
				chem = atof(chem_inp.c_str());
				cin.clear();

				while(!(0 <= chem && chem <= 10)) {
					cout << "	!a point value must be between 0 and 10. please re-enter chemistry point\n";
					chem_inp.clear();
					cout << "chemistry point: ";
					fflush(stdin);
					getline(cin, chem_inp);

					while ((!ValidateFloat(chem_inp))) {
						cin.clear();
						cout << "	!invalid number: " << chem_inp << endl;
						chem_inp.clear();
						cout << "chemistry point: ";
						fflush(stdin);
						getline(cin, chem_inp);
					}
					chem = atof(chem_inp.c_str());
					cin.clear();
				}
		}
	}

	string update_signal_str = "upd:";
	update_signal_str += schoolName;
	update_signal_str += ":";
	update_signal_str += id;
	update_signal_str += ":";
	update_signal_str += name_inp;
	update_signal_str += ":";
	update_signal_str += math_inp;
	update_signal_str += ":";
	update_signal_str += phys_inp;
	update_signal_str += ":";
	update_signal_str += chem_inp;

	char * upd_signal = &update_signal_str[0];
	tcp_send(upd_signal);

	cout << "\n\trecord has been updated successfully!\n";			// if receive updOK signal, print this!
}

// sua thong tin 1 ban ghi
void update_show(string schoolName)
{	
	string s;

	cout << "\nenter the id you want to update info: ";
	fflush(stdin);
	getline(cin,s);
	string update_option;
	cout << "\nproperties you want to update\n-----------------------------\n";
	cout << " n-name | m-math point | p-physics point | c-chemistry point\n";
	cout << "your choices: ";
	fflush(stdin);
	getline(cin, update_option);
	updating(schoolName, s, update_option);
	cin.get();
}

// xoa 1 ban ghi
void del(string schoolName)
{
	string s;
	cout << "\nenter the id you want to delete: ";
	fflush(stdin);
	getline(cin,s);

	char confirm;
	cout << "\n!do you really want to delete this record? [Y/n]: ";
	cin >> confirm;
	if(confirm == 'Y' || confirm == 'y')
	{
		string del_signal_str = "del:";
		del_signal_str += schoolName;
		del_signal_str += ":";
		del_signal_str += s;

		char * del_signal = &del_signal_str[0];
		tcp_send(del_signal);

		cout << "\n\trecord has been deleted successfully!\n";				// if receive delOK signal, print this!
	}
	else
		return;
	cin.get();
}

// showSort()
void sorting(string search_signal)
{
	string sort_choice;
	char * search_sort_signal;
	int sw;
	cout << "\nsorting options\n---------------\n";
	cout << "1-sort by id | 2-sort by name | 3-sort by total of point\n";
	cout << "your choice: ";
	cin >> sw;
	cout << endl;
	switch(sw) {
			   case 1:
				   sort_choice = "|sort:id";
				   search_signal += sort_choice;
				   search_sort_signal = &search_signal[0];
				   tcp_send(search_sort_signal);
				   break;
			   case 2:
				   sort_choice = "|sort:name";
				   search_signal += sort_choice;
				   search_sort_signal = &search_signal[0];
				   tcp_send(search_sort_signal);
				   break;
			   case 3:
				   sort_choice = "|sort:totl";
				   search_signal += sort_choice;
				   search_sort_signal = &search_signal[0];
				   tcp_send(search_sort_signal);
				   break;
	}
}

void search(string schoolName)
{
	string search_signal = "search:";
	search_signal += schoolName;
	search_signal += ":";
	char searchOpt;
		cout << "\npress: 'i' to search with student id\n";
		cout << "       'n' to search with student name\n";
		cout << "       'm' to search with math point\n";
		cout << "       'p' to search with physics point\n";
		cout << "       'c' to search with chemistry point\n";
		cout << "       't' to search with point at total\n";
		cout << "your choice: ";
		cin >> searchOpt;

		string id;
		string name;
		string m;
		string p;
		string c;
		string t;

		switch(searchOpt){
			case 'i':
				search_signal += "ID:";
				cout << "\nenter student id you want to search: ";
				fflush(stdin);
				getline(cin, id);
				search_signal += id;
				sorting(search_signal);
				break;
			case 'n':
				search_signal += "NAME:";
				cout << "\nenter student name you want to search: ";
				fflush(stdin);
				getline(cin, name);
				search_signal += name;
				sorting(search_signal);
				break;
			case 'm':
				search_signal += "MATH:";
				cout << "\nenter math point you want to search: ";
				fflush(stdin);
				getline(cin, m);
				search_signal += m;
				sorting(search_signal);
				break;
			case 'p':
				search_signal += "PHYSICS:";
				cout << "\nenter physics point you want to search: ";
				fflush(stdin);
				getline(cin, p);
				search_signal += p;
				sorting(search_signal);
				break;
			case 'c':
				search_signal += "CHEMISTRY:";
				cout << "\nenter chemistry point you want to search: ";
				fflush(stdin);
				getline(cin, c);
				search_signal += c;
				sorting(search_signal);
				break;
			case 't':
				search_signal += "TOTAL:";
				cout << "\nenter point at total you want to search: ";
				fflush(stdin);
				getline(cin, t);
				search_signal += t;
				sorting(search_signal);
				break;
			default:
				cout << "\n\t!invalid parameter. please choose [i/n/m/p/c/t] to search.. ";
		}
}

void stat(string schoolName)
{
	cout << "\n\t\t---statistics---\n";
	
	string stat_signal_str = "stat:";
	stat_signal_str += schoolName;
	char * stat_signal = &stat_signal_str[0];
	tcp_send(stat_signal);
	cin.get();
}