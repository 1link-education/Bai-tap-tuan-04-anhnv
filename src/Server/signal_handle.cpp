#include "studentIO.h"
#include "student.h"
#include "sqlite_handle.h"

#include <Windows.h>
#include <iostream>
#include <string>
#include <conio.h>
#include <iomanip>
#include <fstream>
#include <stdlib.h>

#define MAXSTD 32
#define MAXPATH 255

using namespace std;

// tao thu muc DIEM_THI_2016 va mot so thu muc temp
void mkDir() {
	CreateDirectory("DIEM_THI_2016", NULL);
	CreateDirectory("DT2016_temp", NULL);
}

// them ban ghi tu ban phim
void addFK(string data)
{
	Student st;
	string schoolName;
	string raw_id;
	string raw_name;
	string raw_add;
	string raw_sex;
	string raw_m;
	string raw_p;
	string raw_c;
	string raw_t;
	size_t delimiter;
	string temp;

	delimiter = data.find_first_of(":");
	schoolName = data.substr(0, delimiter);	cout << "\n\tschoolName: " << schoolName;
	
	temp = data.substr(delimiter+1, data.size());
	data.clear();
	delimiter = temp.find_first_of(":");
	raw_id = temp.substr(0, delimiter); cout << "\n\traw_id: " << raw_id;
	st.setID(raw_id);

	data = temp.substr(delimiter+1, temp.size());
	temp.clear();
	delimiter = data.find_first_of(":");
	raw_name = data.substr(0, delimiter);	cout << "\n\traw_name: " << raw_name;
	st.setName(raw_name);

	temp = data.substr(delimiter+1, data.size());
	data.clear();
	delimiter = temp.find_first_of(":");
	raw_add = temp.substr(0, delimiter);	cout << "\n\traw_add: " << raw_add;
	st.setAdd(raw_add);

	data = temp.substr(delimiter+1, temp.size());
	temp.clear();
	delimiter = data.find_first_of(":");
	raw_sex = data.substr(0, delimiter);	cout << "\n\traw_sex: " << raw_sex;
	st.setSex(raw_sex);

	temp = data.substr(delimiter+1, data.size());
	data.clear();
	delimiter = temp.find_first_of(":");
	raw_m = temp.substr(0, delimiter);	cout << "\n\traw_m: " << raw_m;
	st.setMath(atof(raw_m.c_str()));

	data = temp.substr(delimiter+1, temp.size());
	temp.clear();
	delimiter = data.find_first_of(":");
	raw_p = data.substr(0, delimiter);		cout << "\n\traw_p: " << raw_p;
	st.setPhys(atof(raw_p.c_str()));

	temp = data.substr(delimiter+1, data.size());
	data.clear();
	delimiter = temp.find_first_of(":");
	raw_c = temp.substr(0, delimiter);	cout << "\n\traw_c: " << raw_c;
	st.setChem(atof(raw_c.c_str()));

	data = temp.substr(delimiter+1, temp.size());
	temp.clear();
	delimiter = data.find_first_of(":");
	raw_t = data.substr(0, delimiter);		cout << "\n\traw_t: " << raw_t << endl;
	st.setTotal(atof(raw_t.c_str()));


	sql_insert(schoolName, st);
}

// them ban ghi tu file studentIN.txt
void addFF(string data)
{
	ifstream inFile;
	string m, p, c;

	cout << "\n\timporting records from files to database...\n";
	Sleep(2000);
	inFile.open("studentIN.txt", ios::in);
	if(!inFile)
	{
		cout << "\n\t!file 'studentIN.txt' could not be open!\n";
		return;
	}
	
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
		sql_insert(data, st);
	}
	string last_row_id = "";
	sql_delete(data, last_row_id);				// haha! there was a BUG :v

	inFile.close();
	cout << "\n\tstudent records have successfully imported";
	cout << "\n\tfrom studentIN.txt to table [" << data << "]\n";
}

 // hien thi toan bo cac ban ghi cua truong dang thao tac
void show(string data)
{
	
	Student st;
	
	cout << "\n\n\t\t---show all records---\n\n";
	sql_select(data);
	cin.get();
}

void update(string data)
{
	string tbl_name;
	string id_to_search;
	string argv;
	size_t delimiter;
	string name_inp;
	string set_name;
	string math_inp;
	string set_math;
	string phys_inp;
	string set_phys;
	string chem_inp;
	string set_chem;
	string temp;

	delimiter = data.find_first_of(":");
	tbl_name = data.substr(0, delimiter);	cout << "\n\tschoolName: " << tbl_name;

	temp = data.substr(delimiter+1, data.size());
	data.clear();
	delimiter = temp.find_first_of(":");
	id_to_search = temp.substr(0, delimiter); cout << "\n\tid to search: " << id_to_search;

	data = temp.substr(delimiter+1, temp.size());
	temp.clear();
	delimiter = data.find_first_of(":");
	name_inp = data.substr(0, delimiter);	cout << "\n\traw_name: " << name_inp;
	if(name_inp.compare("") != 0) {
		set_name += "NAME = '";
		set_name += name_inp;
		set_name += "' ";
	}

	temp = data.substr(delimiter+1, data.size());
	data.clear();
	delimiter = temp.find_first_of(":");
	math_inp = temp.substr(0, delimiter);	cout << "\n\traw_m: " << math_inp;
	if((name_inp.compare("") != 0) && (math_inp.compare("") != 0)){
		set_math += ", ";
		set_math += "MATH = '";
		set_math += math_inp;
		set_math += "' ";
	}
	else if((name_inp.compare("") == 0) && (math_inp.compare("") != 0)){
		set_math += "MATH = '";
		set_math += math_inp;
		set_math += "' ";
	}

	data = temp.substr(delimiter+1, temp.size());
	temp.clear();
	delimiter = data.find_first_of(":");
	phys_inp = data.substr(0, delimiter);		cout << "\n\traw_p: " << phys_inp;
	if((math_inp.compare("") != 0) && (phys_inp.compare("") != 0)) {
		set_phys += ", ";
		set_phys += "PHYSICS = '";
		set_phys += phys_inp;
		set_phys += "' ";
	}
	else if((math_inp.compare("") == 0) && (phys_inp.compare("") != 0)) {
		set_phys += "PHYSICS = '";
		set_phys += phys_inp;
		set_phys += "' ";
	}

	temp = data.substr(delimiter+1, data.size());
	data.clear();
	delimiter = temp.find_first_of(":");
	chem_inp = temp.substr(0, delimiter);	cout << "\n\traw_c: " << chem_inp << endl;
	if((phys_inp.compare("") != 0) && (chem_inp.compare("") != 0)) {
		set_chem += ", ";
		set_chem += "CHEMISTRY = '";
		set_chem += chem_inp;
		set_chem += "' ";
	}
	else if((phys_inp.compare("") == 0) && (chem_inp.compare("") != 0)) {
		set_chem += "CHEMISTRY = '";
		set_chem += chem_inp;
		set_chem += "' ";
	}

	sql_update(tbl_name, id_to_search, set_name, set_math, set_phys, set_chem);
}

// xoa 1 ban ghi
void del(string data)
{
	string tbl_name;
	string id_to_del;
	size_t delimiter;
	string temp;
	
	delimiter = data.find_first_of(":");
	tbl_name = data.substr(0, delimiter);	cout << "\n\tschoolName: " << tbl_name;

	temp = data.substr(delimiter+1, data.size());
	data.clear();
	delimiter = temp.find_first_of(":");
	id_to_del = temp.substr(0, delimiter); cout << "\n\tid to search: " << id_to_del << endl;
	sql_delete(tbl_name, id_to_del);
}

void searchID(string schoolName, string key, string temp)
{
	string sortOpt;
	size_t delimiter;
	string sqlSort;

	delimiter = temp.find_first_of(":");
	sortOpt = temp.substr(delimiter+1, temp.size());	cout << "\n\tsortOpt: " << sortOpt << endl;
	if(sortOpt.compare("id") == 0)
		sqlSort = " ORDER BY ID ASC;";
	if(sortOpt.compare("name") == 0)
		sqlSort = " ORDER BY NAME ASC;";
	if(sortOpt.compare("totl") == 0)
		sqlSort = " ORDER BY TOTAL ASC;";

	sql_search_id_sort(schoolName, key, sqlSort);
}

void searchName(string schoolName, string key, string temp)
{
	string sortOpt;
	size_t delimiter;
	string sqlSort;

	delimiter = temp.find_first_of(":");
	sortOpt = temp.substr(delimiter+1, temp.size());	cout << "\n\tsortOpt: " << sortOpt << endl;
	if(sortOpt.compare("id") == 0)
		sqlSort = " ORDER BY ID ASC;";
	if(sortOpt.compare("name") == 0)
		sqlSort = " ORDER BY NAME ASC;";
	if(sortOpt.compare("totl") == 0)
		sqlSort = " ORDER BY TOTAL ASC;";

	sql_search_name_sort(schoolName, key, sqlSort);
}

void searchMath(string schoolName, string key, string temp)
{
	string sortOpt;
	size_t delimiter;
	string sqlSort;

	delimiter = temp.find_first_of(":");
	sortOpt = temp.substr(delimiter+1, temp.size());	cout << "\n\tsortOpt: " << sortOpt << endl;
	if(sortOpt.compare("id") == 0)
		sqlSort = " ORDER BY ID ASC;";
	if(sortOpt.compare("name") == 0)
		sqlSort = " ORDER BY NAME ASC;";
	if(sortOpt.compare("totl") == 0)
		sqlSort = " ORDER BY TOTAL ASC;";

	sql_search_math_sort(schoolName, key, sqlSort);
}

void searchPhysics(string schoolName, string key, string temp)
{
	string sortOpt;
	size_t delimiter;
	string sqlSort;

	delimiter = temp.find_first_of(":");
	sortOpt = temp.substr(delimiter+1, temp.size());	cout << "\n\tsortOpt: " << sortOpt << endl;
	if(sortOpt.compare("id") == 0)
		sqlSort = " ORDER BY ID ASC;";
	if(sortOpt.compare("name") == 0)
		sqlSort = " ORDER BY NAME ASC;";
	if(sortOpt.compare("totl") == 0)
		sqlSort = " ORDER BY TOTAL ASC;";

	sql_search_phys_sort(schoolName, key, sqlSort);
}

void searchChemistry(string schoolName, string key, string temp)
{
	string sortOpt;
	size_t delimiter;
	string sqlSort;

	delimiter = temp.find_first_of(":");
	sortOpt = temp.substr(delimiter+1, temp.size());	cout << "\n\tsortOpt: " << sortOpt << endl;
	if(sortOpt.compare("id") == 0)
		sqlSort = " ORDER BY ID ASC;";
	if(sortOpt.compare("name") == 0)
		sqlSort = " ORDER BY NAME ASC;";
	if(sortOpt.compare("totl") == 0)
		sqlSort = " ORDER BY TOTAL ASC;";

	sql_search_chem_sort(schoolName, key, sqlSort);
}

void searchTotal(string schoolName, string key, string temp)
{
	string sortOpt;
	size_t delimiter;
	string sqlSort;

	delimiter = temp.find_first_of(":");
	sortOpt = temp.substr(delimiter+1, temp.size());	cout << "\n\tsortOpt: " << sortOpt << endl;
	if(sortOpt.compare("id") == 0)
		sqlSort = " ORDER BY ID ASC;";
	if(sortOpt.compare("name") == 0)
		sqlSort = " ORDER BY NAME ASC;";
	if(sortOpt.compare("totl") == 0)
		sqlSort = " ORDER BY TOTAL ASC;";

	sql_search_total_sort(schoolName, key, sqlSort);
}

void search(string data)
{
	size_t delimiter;
	string schoolName;	
	string searchOpt;
	string temp;
	string id_search;
	string name_search;
	string math_search;
	string phys_search;
	string chem_search;
	string totl_search;
	string content_search;

	delimiter = data.find_first_of(":");
	schoolName = data.substr(0, delimiter);	cout << "\n\ttbl_name: " << schoolName;

	temp = data.substr(delimiter+1, data.size());
	data.clear();
	delimiter = temp.find_first_of(":");
	searchOpt = temp.substr(0, delimiter); cout << "\n\tsearchOpt: " << searchOpt;

	data = temp.substr(delimiter+1, temp.size());
	temp.clear();
	delimiter = data.find_first_of("|");
	content_search = data.substr(0, delimiter);	cout << "\n\tcontent_search: " << content_search;

	temp = data.substr(delimiter+1, data.size());

	if(searchOpt.compare("ID") == 0)
		searchID(schoolName, content_search, temp);
	else if(searchOpt.compare("NAME") == 0)
		searchName(schoolName, content_search, temp);
	else if(searchOpt.compare("MATH") == 0)
		searchMath(schoolName, content_search, temp);
	else if(searchOpt.compare("PHYSICS") == 0)
		searchPhysics(schoolName, content_search, temp);
	else if(searchOpt.compare("CHEMISTRY") == 0)
		searchChemistry(schoolName, content_search, temp);
	else if(searchOpt.compare("TOTAL") == 0)
		searchTotal(schoolName, content_search, temp);
}

void stat(string schoolName)
{
	cout << "\n\t\t---statistics---\n";
	sql_stat(schoolName);
	cin.get();
}