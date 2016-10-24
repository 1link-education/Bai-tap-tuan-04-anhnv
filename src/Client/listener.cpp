#include "tcp.h"
#include "student.h"

#include <iostream>
#include <string>

#define DEFAULT_BUFLEN 512

using namespace std;
using std::string;

void prepare_for_show(string data_to_handle)
{
	//cout << "\n\n\tstd_from_server: " << data_to_handle << endl;
	size_t delimiter = data_to_handle.find_first_of(":");
	string temp = data_to_handle.substr(delimiter+1, data_to_handle.size());
	data_to_handle.clear();
	
	string std_number;
	delimiter = temp.find_first_of(":");
	std_number = temp.substr(0, delimiter); 
	cout << "\n\nstudent number: " << std_number;
	cout << "\n-----------------\n";

	string std_id;
	data_to_handle = temp.substr(delimiter+1, temp.size());
	temp.clear();
	delimiter = data_to_handle.find_first_of(":");
	std_id = data_to_handle.substr(0, delimiter);
	cout << "id: " << std_id << endl;

	string std_name;
	temp = data_to_handle.substr(delimiter+1, data_to_handle.size());
	data_to_handle.clear();
	delimiter = temp.find_first_of(":");
	std_name = temp.substr(0, delimiter);
	cout << "name: " << std_name << endl;

	string std_add;
	data_to_handle = temp.substr(delimiter+1, temp.size());
	temp.clear();
	delimiter = data_to_handle.find_first_of(":");
	std_add = data_to_handle.substr(0, delimiter);
	cout << "address: " << std_add << endl;

	string std_sex;
	temp = data_to_handle.substr(delimiter+1, data_to_handle.size());
	data_to_handle.clear();
	delimiter = temp.find_first_of(":");
	std_sex = temp.substr(0, delimiter);
	cout << "sex: " << std_sex << endl;

	string std_m;
	data_to_handle = temp.substr(delimiter+1, temp.size());
	temp.clear();
	delimiter = data_to_handle.find_first_of(":");
	std_m = data_to_handle.substr(0, delimiter);
	cout << "math point: " << std_m << endl;

	string std_p;
	temp = data_to_handle.substr(delimiter+1, data_to_handle.size());
	data_to_handle.clear();
	delimiter = temp.find_first_of(":");
	std_p = temp.substr(0, delimiter);
	cout << "physics point: " << std_p << endl;

	string std_c;
	data_to_handle = temp.substr(delimiter+1, temp.size());
	temp.clear();
	delimiter = data_to_handle.find_first_of(":");
	std_c = data_to_handle.substr(0, delimiter);
	cout << "chemistry point: " << std_c << endl;

	string std_t;
	temp = data_to_handle.substr(delimiter+1, data_to_handle.size());
	data_to_handle.clear();
	delimiter = temp.find_first_of(":");
	std_t = temp.substr(0, delimiter);
	cout << "point at total: " << std_t << endl << endl;
}