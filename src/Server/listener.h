#ifndef _LISTENER_H_
#define _LISTENER_H_

#include "tcp.h"

#include <string>

using std::string;

void listen_for_schl(string);
void server_listener(char[], int);

#endif