#ifndef _STUDENTSQLITE_H_
#define _STUDENTSQLITE_H_

#include "student.h"

static int callback(void *, int, char **, char **);
void sql_init_db();
void sql_create(string);
void sql_insert(string, Student);
void sql_select(string);
void sql_update(string, string, string, string, string, string);
void sql_delete(string, string);
void sql_search_id(string, string);
void sql_search_id_sort(string, string, string);
void sql_search_name_sort(string, string, string);
void sql_search_math_sort(string, string, string);
void sql_search_phys_sort(string, string, string);
void sql_search_chem_sort(string, string, string);
void sql_search_total_sort(string, string, string);
void sql_stat(string);

#endif