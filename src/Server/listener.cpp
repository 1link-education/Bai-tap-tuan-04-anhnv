#include "tcp.h"
#include "student.h"
#include "signal_handle.h"
#include "sqlite_handle.h"

#include <iostream>
#include <string>

#define DEFAULT_BUFLEN 512

using namespace std;
using std::string;

void listen_for_schl(string schoolName)
{
	sql_create(schoolName);
}

void server_listener(char ANALYSIS[DEFAULT_BUFLEN], int bytecount)
{
	string ANALYSIS_STR;
	int i;
	for(i = 0; i < bytecount; i++)												// char[] to string
		if(ANALYSIS[i] != NULL)
			ANALYSIS_STR += ANALYSIS[i];
	cout << "\n\n\trequest_from_client: " << ANALYSIS_STR << endl;
	size_t delimiter = ANALYSIS_STR.find_first_of(":");
	string signal_token = ANALYSIS_STR.substr(0, delimiter);
	cout << "\tsignal_token       : " << signal_token << endl;
	string data_to_handle = ANALYSIS_STR.substr(delimiter + 1, ANALYSIS_STR.size());
	cout << "\tdata_to_handle     : " << data_to_handle << endl << endl;

	if(signal_token.compare("aFK") == 0)
		addFK(data_to_handle);
	else if(signal_token.compare("aFF") == 0)
		addFF(data_to_handle);
	else if (signal_token.compare("shw") == 0)
		show(data_to_handle);
	else if(signal_token.compare("upd") == 0)
		update(data_to_handle);
	else if(signal_token.compare("del") == 0)
		del(data_to_handle);
	else if(signal_token.compare("search") == 0)
		search(data_to_handle);
	else if(signal_token.compare("stat") == 0)
		stat(data_to_handle);
}