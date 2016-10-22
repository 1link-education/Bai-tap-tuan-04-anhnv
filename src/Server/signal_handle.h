#ifndef _SIGNAL_HANDLE_H_
#define _SIGNAL_HANDLE_H_

#include "student.h"


void mkDir();
string Schooling();
bool checkID(string, int, Student);
void addFK(string);
void addFF(string);
void show(string);
void update(string);
void searchID(string, string, string);
void searchName(string, string, string);
void searchMath(string, string, string);
void searchPhysics(string, string, string);
void searchChemistry(string, string, string);
void searchTotal(string, string, string);
void search(string);
void sortID(int, Student[]);
void sortName(int, Student[]);
void sortTotal(int, Student[]);
string sorting();
void del(string);
void stat(string);

#endif