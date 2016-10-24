#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <conio.h>
#include "sqlite3.h"
#include "student.h"
#include "tcp.h"

using std::string;
using namespace std;

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	for(i=0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

void sql_init_db()
{
	{
		sqlite3 *db;
		char *zErrMsg = 0;
		int rc;

		rc = sqlite3_open("DIEM_THI_2016.db", &db);

		if( rc ){
			//fprintf(stderr, "\n\tdatabase could not be opened: %s\n", sqlite3_errmsg(db));
			exit(0);
		}/*else{
			fprintf(stderr, "\n\topened database successfully!\n");
		}*/
		sqlite3_close(db);
	}
}

void sql_create(string tbl_name)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;
	char *sql;
	string sqlString;

	rc = sqlite3_open("DIEM_THI_2016.db", &db);
	if( rc ){
		fprintf(stderr, "\n\tdatabase could not be opened: %s\n", sqlite3_errmsg(db));
		exit(0);
	}else{
		fprintf(stdout, "\n\topened database successfully!\n\n");
	}

	sqlString = "CREATE TABLE ";						//beware of last whitespace!!! [CREATE TABLE ]
	sqlString += tbl_name;
	sqlString += "("  \
		"ID				VARCHAR(10) PRIMARY KEY    NOT NULL," \
		"NAME           VARCHAR(50)    NOT NULL," \
		"ADDRESS        VARCHAR(50)," \
		"SEX            VARCHAR(10)     NOT NULL," \
		"MATH           FLOAT     NOT NULL," \
		"PHYSICS        FLOAT     NOT NULL," \
		"CHEMISTRY      FLOAT     NOT NULL," \
		"TOTAL			FLOAT	  NOT NULL);";
	sql = &sqlString[0];								// string to char*, confirmed. don't use sqlString.c_str()

	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "\n\tSQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "\n\ttable created successfully!\n");
	}

	sqlite3_close(db);
}

void sql_insert(string tbl_name, Student st)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;
	char *sql;
	string sqlString;

	stringstream stream;
	string math_string;
	stream << st.getMath();
	math_string = stream.str();
	stream.str(string());								// to clear string stream buffer
	stream.clear();

	string phys_string;
	stream << st.getPhys();
	phys_string = stream.str();
	stream.str(string());
	stream.clear();

	string chem_string;
	stream << st.getChem();
	chem_string = stream.str();
	stream.str(string());
	stream.clear();

	string totl_string;
	stream << st.getTotal();
	totl_string = stream.str();
	stream.str(string());
	stream.clear();

	rc = sqlite3_open("DIEM_THI_2016.db", &db);
	if( rc ){
		fprintf(stderr, "\n\tdatabase could not be opened: %s\n", sqlite3_errmsg(db));
		exit(0);
	}else{
		fprintf(stdout, "\n\topened database successfully!\n\n");
	}
	
	sqlString = "INSERT INTO ";
	sqlString += tbl_name;
	sqlString += " (ID,NAME,ADDRESS,SEX,MATH,PHYSICS,CHEMISTRY,TOTAL) VALUES ('";
	sqlString += st.id;
	sqlString += "', '";
	sqlString += st.getName();
	sqlString += "', '";
	sqlString += st.getAdd();
	sqlString += "', '";
	sqlString += st.getSex();
	sqlString += "', ";
	sqlString += math_string;
	sqlString += ", ";
	sqlString += phys_string;
	sqlString += ", ";
	sqlString += chem_string;
	sqlString += ", ";
	sqlString += totl_string;
	sqlString += ");";
	sql = &sqlString[0];

	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "\n\tSQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "\n\trecords inserted to table successfully!\n");
	}

	sqlite3_close(db);
}

void sql_select(string tbl_name)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;
	char *sql;
	const char* data = "Callback function called";
	string sqlString;

	rc = sqlite3_open("DIEM_THI_2016.db", &db);
	if( rc ){
		fprintf(stderr, "\n\tdatabase could not be opened: %s\n", sqlite3_errmsg(db));
		exit(0);
	}else{
		fprintf(stdout, "\n\topened database successfully!\n\n");
	}

	sqlString = "SELECT * from ";
	sqlString += tbl_name;
	sql = &sqlString[0];

	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "\n\toperation done successfully!\n");
	}

	char **results = NULL;
	int rows, columns;

	sqlite3_get_table(db, sql, &results, &rows, &columns, &zErrMsg);
	if (rc)
	{
		cerr << "\n\terror executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(zErrMsg);
	}
	else
	{
		for (int rowCtr = 1; rowCtr <= rows; ++rowCtr)
		{
			int cell_position = rowCtr * columns;
			stringstream stream;
			string table_content_str = "STD:";
			char * table_content_char;

			stream << rowCtr;
			table_content_str += stream.str();
			stream.str(string());
			stream.clear();

			table_content_str += ":";
			table_content_str += results[cell_position];
			table_content_str += ":";
			table_content_str += results[cell_position + 1];
			table_content_str += ":";
			table_content_str += results[cell_position + 2];
			table_content_str += ":";
			table_content_str += results[cell_position + 3];
			table_content_str += ":";
			table_content_str += results[cell_position + 4];
			table_content_str += ":";
			table_content_str += results[cell_position + 5];
			table_content_str += ":";	
			table_content_str += results[cell_position + 6];

			table_content_char = &table_content_str[0];
			tcp_send(table_content_char);
			cout << "\nsent: " << table_content_char << endl;
		}
	}
	sqlite3_free_table(results);

	sqlite3_close(db);
}

void sql_search_id(string tbl_name, string keyword)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;
	char *sql;
	const char* data = "Callback function called";
	string sqlString;

	rc = sqlite3_open("DIEM_THI_2016.db", &db);
	if( rc ){
		fprintf(stderr, "\n\tdatabase could not be opened: %s\n", sqlite3_errmsg(db));
		exit(0);
	}else{
		fprintf(stdout, "\n\topened database successfully!\n\n");
	}

	sqlString = "SELECT * from ";
	sqlString += tbl_name;
	//sqlString += " WHERE ID GLOB '";
	sqlString += " WHERE ID = '";
	sqlString += keyword;
	//sqlString += "*';";
	sqlString += "';";
	sql = &sqlString[0];

	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "\n\tSQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "\n\toperation done successfully!\n");
	}

	char **results = NULL;
	int rows, columns;

	sqlite3_get_table(db, sql, &results, &rows, &columns, &zErrMsg);
	if (rc)
	{
		cerr << "\n\terror executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(zErrMsg);
	}
	else
	{
		for (int rowCtr = 1; rowCtr <= rows; ++rowCtr)
		{
			int cell_position = rowCtr * columns;
			stringstream stream;
			string table_content_str = "STD:";
			char * table_content_char;

			stream << rowCtr;
			table_content_str += stream.str();
			stream.str(string());
			stream.clear();

			table_content_str += ":";
			table_content_str += results[cell_position];
			table_content_str += ":";
			table_content_str += results[cell_position + 1];
			table_content_str += ":";
			table_content_str += results[cell_position + 2];
			table_content_str += ":";
			table_content_str += results[cell_position + 3];
			table_content_str += ":";
			table_content_str += results[cell_position + 4];
			table_content_str += ":";
			table_content_str += results[cell_position + 5];
			table_content_str += ":";	
			table_content_str += results[cell_position + 6];

			table_content_char = &table_content_str[0];
			tcp_send(table_content_char);
			cout << "\nsent: " << table_content_char << endl;
		}
	}

	sqlite3_free_table(results);
	sqlite3_close(db);
}

void sql_update(string tbl_name, string id_to_update, string set_name, string set_math, string set_phys, string set_chem)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;
	char *sql;
	const char* data = "Callback function called";
	string sqlString;
	
	rc = sqlite3_open("DIEM_THI_2016.db", &db);
	if( rc ){
		fprintf(stderr, "\n\tdatabase could not be opened: %s\n", sqlite3_errmsg(db));
		exit(0);
	}else{
		fprintf(stdout, "\n\topened database successfully!\n\n");
	}

	sqlString = "UPDATE ";
	sqlString += tbl_name;
	sqlString += " set ";
	sqlString += set_name;
	sqlString += set_math;
	sqlString += set_phys;
	sqlString += set_chem;
	sqlString += "WHERE ID = '";
	sqlString += id_to_update;
	sqlString += "'; SELECT * from ";
	sqlString += tbl_name;
	sql = &sqlString[0];

	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "\n\tSQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "\n\toperation done successfully!\n");
	}

	sql_search_id(tbl_name, id_to_update);

	sqlite3_close(db);
}

void sql_delete(string tbl_name, string id_to_delete)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;
	char *sql;
	const char* data = "Callback function called";
	string sqlString;
	
	sql_search_id(tbl_name, id_to_delete);

	rc = sqlite3_open("DIEM_THI_2016.db", &db);
	if( rc ){
		fprintf(stderr, "\n\tdatabase could not be opened: %s\n", sqlite3_errmsg(db));
		exit(0);
	}else{
		fprintf(stdout, "\n\topened database successfully\n\n");
	}

	sqlString = "DELETE from ";
	sqlString += tbl_name;
	sqlString += " where ID = '";
	sqlString += id_to_delete;
	sqlString += "'; SELECT * from ";
	sqlString += tbl_name;
	sql = &sqlString[0];

	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "\n\toperation done successfully\n");
	}

	sqlite3_close(db);
}

void sql_search_id_sort(string tbl_name, string keyword, string sort)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;
	char *sql;
	const char* data = "Callback function called";
	string sqlString;

	rc = sqlite3_open("DIEM_THI_2016.db", &db);
	if( rc ){
		fprintf(stderr, "\n\tdatabase could not be opened: %s\n", sqlite3_errmsg(db));
		exit(0);
	}else{
		fprintf(stdout, "\n\topened database successfully!\n\n");
	}

	sqlString = "SELECT * from ";
	sqlString += tbl_name;
	sqlString += " WHERE ID GLOB '";
	sqlString += keyword;
	sqlString += "*'";
	sqlString += sort;
	sql = &sqlString[0];

	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "\n\tSQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "\n\toperation done successfully!\n");
	}

	char **results = NULL;
	int rows, columns;

	sqlite3_get_table(db, sql, &results, &rows, &columns, &zErrMsg);
	if (rc)
	{
		cerr << "\n\terror executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(zErrMsg);
	}
	else
	{
		for (int rowCtr = 1; rowCtr <= rows; ++rowCtr)
		{
			int cell_position = rowCtr * columns;
			stringstream stream;
			string table_content_str = "STD:";
			char * table_content_char;

			stream << rowCtr;
			table_content_str += stream.str();
			stream.str(string());
			stream.clear();

			table_content_str += ":";
			table_content_str += results[cell_position];
			table_content_str += ":";
			table_content_str += results[cell_position + 1];
			table_content_str += ":";
			table_content_str += results[cell_position + 2];
			table_content_str += ":";
			table_content_str += results[cell_position + 3];
			table_content_str += ":";
			table_content_str += results[cell_position + 4];
			table_content_str += ":";
			table_content_str += results[cell_position + 5];
			table_content_str += ":";	
			table_content_str += results[cell_position + 6];

			table_content_char = &table_content_str[0];
			tcp_send(table_content_char);
			cout << "\nsent: " << table_content_char << endl;
		}
	}

	sqlite3_free_table(results);
	sqlite3_close(db);
}

void sql_search_name_sort(string tbl_name, string keyword, string sort)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;
	char *sql;
	const char* data = "Callback function called";
	string sqlString;

	rc = sqlite3_open("DIEM_THI_2016.db", &db);
	if( rc ){
		fprintf(stderr, "\n\tdatabase could not be opened: %s\n", sqlite3_errmsg(db));
		exit(0);
	}else{
		fprintf(stdout, "\n\topened database successfully!\n\n");
	}

	sqlString = "SELECT * from ";
	sqlString += tbl_name;
	sqlString += " WHERE NAME GLOB '";
	sqlString += keyword;
	sqlString += "*'";
	sqlString += sort;
	sql = &sqlString[0];

	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "\n\tSQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "\n\toperation done successfully!\n");
	}

	char **results = NULL;
	int rows, columns;

	sqlite3_get_table(db, sql, &results, &rows, &columns, &zErrMsg);
	if (rc)
	{
		cerr << "\n\terror executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(zErrMsg);
	}
	else
	{
		for (int rowCtr = 1; rowCtr <= rows; ++rowCtr)
		{
			int cell_position = rowCtr * columns;
			stringstream stream;
			string table_content_str = "STD:";
			char * table_content_char;

			stream << rowCtr;
			table_content_str += stream.str();
			stream.str(string());
			stream.clear();

			table_content_str += ":";
			table_content_str += results[cell_position];
			table_content_str += ":";
			table_content_str += results[cell_position + 1];
			table_content_str += ":";
			table_content_str += results[cell_position + 2];
			table_content_str += ":";
			table_content_str += results[cell_position + 3];
			table_content_str += ":";
			table_content_str += results[cell_position + 4];
			table_content_str += ":";
			table_content_str += results[cell_position + 5];
			table_content_str += ":";	
			table_content_str += results[cell_position + 6];

			table_content_char = &table_content_str[0];
			tcp_send(table_content_char);
			cout << "\nsent: " << table_content_char << endl;
		}
	}

	sqlite3_free_table(results);
	sqlite3_close(db);
}

void sql_search_math_sort(string tbl_name, string keyword, string sort)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;
	char *sql;
	const char* data = "Callback function called";
	string sqlString;

	rc = sqlite3_open("DIEM_THI_2016.db", &db);
	if( rc ){
		fprintf(stderr, "\n\tdatabase could not be opened: %s\n", sqlite3_errmsg(db));
		exit(0);
	}else{
		fprintf(stdout, "\n\topened database successfully!\n\n");
	}

	sqlString = "SELECT * from ";
	sqlString += tbl_name;
	sqlString += " WHERE MATH GLOB '";
	sqlString += keyword;
	sqlString += "*'";
	sqlString += sort;
	sql = &sqlString[0];

	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "\n\tSQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "\n\toperation done successfully!\n");
	}

	char **results = NULL;
	int rows, columns;

	sqlite3_get_table(db, sql, &results, &rows, &columns, &zErrMsg);
	if (rc)
	{
		cerr << "\n\terror executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(zErrMsg);
	}
	else
	{
		for (int rowCtr = 1; rowCtr <= rows; ++rowCtr)
		{
			int cell_position = rowCtr * columns;
			stringstream stream;
			string table_content_str = "STD:";
			char * table_content_char;

			stream << rowCtr;
			table_content_str += stream.str();
			stream.str(string());
			stream.clear();

			table_content_str += ":";
			table_content_str += results[cell_position];
			table_content_str += ":";
			table_content_str += results[cell_position + 1];
			table_content_str += ":";
			table_content_str += results[cell_position + 2];
			table_content_str += ":";
			table_content_str += results[cell_position + 3];
			table_content_str += ":";
			table_content_str += results[cell_position + 4];
			table_content_str += ":";
			table_content_str += results[cell_position + 5];
			table_content_str += ":";	
			table_content_str += results[cell_position + 6];

			table_content_char = &table_content_str[0];
			tcp_send(table_content_char);
			cout << "\nsent: " << table_content_char << endl;
		}
	}

	sqlite3_free_table(results);
	sqlite3_close(db);
}

void sql_search_phys_sort(string tbl_name, string keyword, string sort)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;
	char *sql;
	const char* data = "Callback function called";
	string sqlString;

	rc = sqlite3_open("DIEM_THI_2016.db", &db);
	if( rc ){
		fprintf(stderr, "\n\tdatabase could not be opened: %s\n", sqlite3_errmsg(db));
		exit(0);
	}else{
		fprintf(stdout, "\n\topened database successfully!\n\n");
	}

	sqlString = "SELECT * from ";
	sqlString += tbl_name;
	sqlString += " WHERE PHYSICS GLOB '";
	sqlString += keyword;
	sqlString += "*'";
	sqlString += sort;
	sql = &sqlString[0];

	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "\n\tSQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "\n\toperation done successfully!\n");
	}

	char **results = NULL;
	int rows, columns;

	sqlite3_get_table(db, sql, &results, &rows, &columns, &zErrMsg);
	if (rc)
	{
		cerr << "\n\terror executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(zErrMsg);
	}
	else
	{
		for (int rowCtr = 1; rowCtr <= rows; ++rowCtr)
		{
			int cell_position = rowCtr * columns;
			stringstream stream;
			string table_content_str = "STD:";
			char * table_content_char;

			stream << rowCtr;
			table_content_str += stream.str();
			stream.str(string());
			stream.clear();

			table_content_str += ":";
			table_content_str += results[cell_position];
			table_content_str += ":";
			table_content_str += results[cell_position + 1];
			table_content_str += ":";
			table_content_str += results[cell_position + 2];
			table_content_str += ":";
			table_content_str += results[cell_position + 3];
			table_content_str += ":";
			table_content_str += results[cell_position + 4];
			table_content_str += ":";
			table_content_str += results[cell_position + 5];
			table_content_str += ":";	
			table_content_str += results[cell_position + 6];

			table_content_char = &table_content_str[0];
			tcp_send(table_content_char);
			cout << "\nsent: " << table_content_char << endl;
		}
	}

	sqlite3_free_table(results);
	sqlite3_close(db);
}

void sql_search_chem_sort(string tbl_name, string keyword, string sort)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;
	char *sql;
	const char* data = "Callback function called";
	string sqlString;

	rc = sqlite3_open("DIEM_THI_2016.db", &db);
	if( rc ){
		fprintf(stderr, "\n\tdatabase could not be opened: %s\n", sqlite3_errmsg(db));
		exit(0);
	}else{
		fprintf(stdout, "\n\topened database successfully!\n\n");
	}

	sqlString = "SELECT * from ";
	sqlString += tbl_name;
	sqlString += " WHERE CHEMISTRY GLOB '";
	sqlString += keyword;
	sqlString += "*'";
	sqlString += sort;
	sql = &sqlString[0];

	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "\n\tSQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "\n\toperation done successfully!\n");
	}

	char **results = NULL;
	int rows, columns;

	sqlite3_get_table(db, sql, &results, &rows, &columns, &zErrMsg);
	if (rc)
	{
		cerr << "\n\terror executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(zErrMsg);
	}
	else
	{
		for (int rowCtr = 1; rowCtr <= rows; ++rowCtr)
		{
			int cell_position = rowCtr * columns;
			stringstream stream;
			string table_content_str = "STD:";
			char * table_content_char;

			stream << rowCtr;
			table_content_str += stream.str();
			stream.str(string());
			stream.clear();

			table_content_str += ":";
			table_content_str += results[cell_position];
			table_content_str += ":";
			table_content_str += results[cell_position + 1];
			table_content_str += ":";
			table_content_str += results[cell_position + 2];
			table_content_str += ":";
			table_content_str += results[cell_position + 3];
			table_content_str += ":";
			table_content_str += results[cell_position + 4];
			table_content_str += ":";
			table_content_str += results[cell_position + 5];
			table_content_str += ":";	
			table_content_str += results[cell_position + 6];

			table_content_char = &table_content_str[0];
			tcp_send(table_content_char);
			cout << "\nsent: " << table_content_char << endl;
		}
	}

	sqlite3_free_table(results);
	sqlite3_close(db);
}

void sql_search_total_sort(string tbl_name, string keyword, string sort)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;
	char *sql;
	const char* data = "Callback function called";
	string sqlString;

	rc = sqlite3_open("DIEM_THI_2016.db", &db);
	if( rc ){
		fprintf(stderr, "\n\tdatabase could not be opened: %s\n", sqlite3_errmsg(db));
		exit(0);
	}else{
		fprintf(stdout, "\n\topened database successfully!\n\n");
	}

	sqlString = "SELECT * from ";
	sqlString += tbl_name;
	sqlString += " WHERE TOTAL GLOB '";
	sqlString += keyword;
	sqlString += "*'";
	sqlString += sort;
	sql = &sqlString[0];

	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr, "\n\tSQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "\n\toperation done successfully!\n");
	}

	char **results = NULL;
	int rows, columns;

	sqlite3_get_table(db, sql, &results, &rows, &columns, &zErrMsg);
	if (rc)
	{
		cerr << "\n\terror executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_free(zErrMsg);
	}
	else
	{
		for (int rowCtr = 1; rowCtr <= rows; ++rowCtr)
		{
			int cell_position = rowCtr * columns;
			stringstream stream;
			string table_content_str = "STD:";
			char * table_content_char;

			stream << rowCtr;
			table_content_str += stream.str();
			stream.str(string());
			stream.clear();

			table_content_str += ":";
			table_content_str += results[cell_position];
			table_content_str += ":";
			table_content_str += results[cell_position + 1];
			table_content_str += ":";
			table_content_str += results[cell_position + 2];
			table_content_str += ":";
			table_content_str += results[cell_position + 3];
			table_content_str += ":";
			table_content_str += results[cell_position + 4];
			table_content_str += ":";
			table_content_str += results[cell_position + 5];
			table_content_str += ":";	
			table_content_str += results[cell_position + 6];

			table_content_char = &table_content_str[0];
			tcp_send(table_content_char);
			cout << "\nsent: " << table_content_char << endl;
		}
	}

	sqlite3_free_table(results);
	sqlite3_close(db);
}

extern "C" int callback_count(void* data, int count, char** rows,char**)
{
	if (count == 1 && rows) {
		*static_cast<int*>(data) = atoi(rows[0]);
		return 0;
	}
	return 1;
}

void sql_stat(string tbl_name)
{
	int count1 = 0, count2 = 0, count3 = 0, count4 = 0;

	sqlite3 *db;
	char *zErrMsg = 0;
	int  rc;
	char *sql;
	const char* data = "Callback function called";
	string sqlString;

	rc = sqlite3_open("DIEM_THI_2016.db", &db);
	if( rc ){
		fprintf(stderr, "\n\tdatabase could not be opened: %s\n", sqlite3_errmsg(db));
		exit(0);
	}else{
		fprintf(stdout, "\n\topened database successfully!\n\n");
	}
	cout << "\nnumber of students whose total point is..\n\n";
	
	string send_str = "\n\nnumber of students whose total point is..\n\n";
	send_str += "-under 15         : ";

	cout << "-under 15         : ";
	sqlString = "SELECT count(TOTAL) FROM ";
	sqlString += tbl_name;
	sqlString += " WHERE TOTAL <= 15;";
	sql = &sqlString[0];
	rc = sqlite3_exec(db, sql, callback_count, &count1, &zErrMsg);
	cout << count1 << endl;

	stringstream ss;
	string str;
	ss << count1;
	ss >> str;
	send_str += str;
	ss.clear();
	str.clear();

	send_str += "\n-between 15 and 20: ";

	cout << "-between 15 and 20: ";
	sqlString = "";
	sqlString = "SELECT count(TOTAL) FROM ";
	sqlString += tbl_name;
	sqlString += " WHERE TOTAL > 15 AND TOTAL <= 20;";
	sql = &sqlString[0];
	rc = sqlite3_exec(db, sql, callback_count, &count2, &zErrMsg);
	cout << count2 << endl;

	ss << count2;
	ss >> str;
	send_str += str;
	ss.clear();
	str.clear();

	send_str += "\n-between 20 and 25: ";

	cout << "-between 20 and 25: ";
	sqlString = "";
	sqlString = "SELECT count(TOTAL) FROM ";
	sqlString += tbl_name;
	sqlString += " WHERE TOTAL > 20 AND TOTAL <= 25;";
	sql = &sqlString[0];
	rc = sqlite3_exec(db, sql, callback_count, &count3, &zErrMsg);
	cout << count3 << endl;

	ss << count3;
	ss >> str;
	send_str += str;
	ss.clear();
	str.clear();

	send_str += "\n-higher than 25   : ";

	cout << "-higher than 25   : ";
	sqlString = "";
	sqlString = "SELECT count(TOTAL) FROM ";
	sqlString += tbl_name;
	sqlString += " WHERE TOTAL > 25;";
	sql = &sqlString[0];
	rc = sqlite3_exec(db, sql, callback_count, &count4, &zErrMsg);
	cout << count4 << endl;

	ss << count4;
	ss >> str;
	send_str += str;
	send_str += "\n";
	ss.clear();
	str.clear();

	char * send_char = &send_str[0];

	tcp_send(send_char);

	sqlite3_close(db);
}