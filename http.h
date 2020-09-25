#pragma once
#ifndef HTTP_H
#define HTTP_H

#include <stdio.h>
using namespace std;

string send_get(string host, string endpoint);
string send_post(string host, string endpoint, string data);


#endif