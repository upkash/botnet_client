#include <stdio.h>

#include <string>
#include <iostream>
#include <windows.h>
#include <winhttp.h>
#include "http.h"
#pragma comment(lib, "winhttp.lib")
using namespace std;
//slave node formulate request for the master given endpoint, req type, and data(json) for post
wstring s2ws(const string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    wstring r(buf);
    delete[] buf;
    return r;
}


string send_get(string host, string endpoint) {
    wstring url = s2ws(endpoint);
    wstring domain = s2ws(host);
    string result;
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;
    BOOL  bResults = FALSE;
    HINTERNET  hSession = NULL,
        hConnect = NULL,
        hRequest = NULL;

    // Use WinHttpOpen to obtain a session handle.
    hSession = WinHttpOpen(L"WinHTTP Example/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS, 0);
    cout << "Opened WinHttp Session" << endl;
    // Specify an HTTP server.
    if (hSession)
        hConnect = WinHttpConnect(hSession, domain.c_str(),
            INTERNET_DEFAULT_HTTP_PORT, 0);

    // Create an HTTP request handle.
    if (hConnect)
        hRequest = WinHttpOpenRequest(hConnect, L"GET", url.c_str(),
            NULL, WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            0);
    cout << "Opened a request" << endl;
    // Send a request.
    if (hRequest)
        bResults = WinHttpSendRequest(hRequest,
            WINHTTP_NO_ADDITIONAL_HEADERS,
            0, WINHTTP_NO_REQUEST_DATA, 0,
            0, 0);
    cout << "Sent the request" << endl;

    // End the request.
    if (bResults)
        bResults = WinHttpReceiveResponse(hRequest, NULL);

    // Keep checking for data until there is nothing left.
    if (bResults)
        do
        {
            // Check for available data.
            dwSize = 0;
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
                printf("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());

            // Allocate space for the buffer.
            pszOutBuffer = new char[dwSize + 1];
            if (!pszOutBuffer)
            {
                printf("Out of memory\n");
                dwSize = 0;
            }
            else
            {
                // Read the Data.
                ZeroMemory(pszOutBuffer, dwSize + 1);

                if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
                    dwSize, &dwDownloaded))
                    printf("Error %u in WinHttpReadData.\n", GetLastError());
                else
                    //printf("%s\n", pszOutBuffer);
                    result += pszOutBuffer;

                // Free the memory allocated to the buffer.
                delete[] pszOutBuffer;
            }

        } while (dwSize > 0);


        // Report any errors.
        if (!bResults)
            //printf("Error %d has occurred.\n", GetLastError());
            return "ERROR";

        // Close any open handles.
        if (hRequest) WinHttpCloseHandle(hRequest);
        if (hConnect) WinHttpCloseHandle(hConnect);
        if (hSession) WinHttpCloseHandle(hSession);
        return result;
}

string send_post(string host, string endpoint, string data) {
    wstring url = s2ws(endpoint);
    wstring domain = s2ws(host);
    //wstring wdata = s2ws(data);
    LPCSTR wdata = data.c_str();
    string result;
    DWORD data_len = data.length();
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;
    BOOL  bResults = FALSE;
    HINTERNET  hSession = NULL,
        hConnect = NULL,
        hRequest = NULL;

    // Use WinHttpOpen to obtain a session handle.
    hSession = WinHttpOpen(L"WinHTTP Example/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS, 0);
    cout << "Opened WinHttp Session" << endl;
    // Specify an HTTP server.
    if (hSession)
        hConnect = WinHttpConnect(hSession, domain.c_str(),
            INTERNET_DEFAULT_HTTP_PORT, 0);

    // Create an HTTP request handle.
    if (hConnect)
        hRequest = WinHttpOpenRequest(hConnect, L"POST", url.c_str(),
            NULL, WINHTTP_NO_REFERER,
            WINHTTP_DEFAULT_ACCEPT_TYPES,
            0);
    cout << "Opened a request" << endl;
    // Send a request.
    if (hRequest)
        bResults = WinHttpSendRequest(hRequest,
            L"Content-Type: application/json\r\n",
            0, (LPVOID)wdata, data_len,
            data_len, 0);
    cout << "Sent the request" << endl;

    // End the request.
    if (bResults)
        bResults = WinHttpReceiveResponse(hRequest, NULL);

    // Keep checking for data until there is nothing left.
    if (bResults)
        do
        {
            // Check for available data.
            dwSize = 0;
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
                printf("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());

            // Allocate space for the buffer.
            pszOutBuffer = new char[dwSize + 1];
            if (!pszOutBuffer)
            {
                printf("Out of memory\n");
                dwSize = 0;
            }
            else
            {
                // Read the Data.
                ZeroMemory(pszOutBuffer, dwSize + 1);

                if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
                    dwSize, &dwDownloaded))
                    printf("Error %u in WinHttpReadData.\n", GetLastError());
                else
                    //printf("%s\n", pszOutBuffer);
                    result += pszOutBuffer;

                // Free the memory allocated to the buffer.
                delete[] pszOutBuffer;
            }

        } while (dwSize > 0);


        // Report any errors.
        if (!bResults)
            //printf("Error %d has occurred.\n", GetLastError());
            return "ERROR";

        // Close any open handles.
        if (hRequest) WinHttpCloseHandle(hRequest);
        if (hConnect) WinHttpCloseHandle(hConnect);
        if (hSession) WinHttpCloseHandle(hSession);
        return result;
}
