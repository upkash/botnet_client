#pragma once
#ifndef HTTP_H
#define HTTP_H

#include <stdio.h>
using namespace std;

wstring s2ws(const string& s);
string url_encode(const string& value);
string send_get(string host, string endpoint);
string send_post(string host, string endpoint, string data_type, string data);

#endif