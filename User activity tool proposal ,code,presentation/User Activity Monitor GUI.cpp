#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <set>
#include <map>
#include <fstream>
#include <ctime>

using namespace std;

#define ID_BTN_START 101
#define ID_BTN_STOP 102
#define ID_EDIT_LOG 103
#define ID_TIMER 1

HWND hEditLog, hBtnStart, hBtnStop;

set<DWORD> prevPIDs;
map<DWORD, time_t> startTimes;
map<DWORD, string> processNames;

bool monitoring = false;
const string logFile = "activity_log.txt";

// Time function
string getTimeStr() {
    time_t now = time(0);
    char buf[26];
    ctime_s(buf, sizeof(buf), &now);
    string s(buf);
    if (!s.empty() && s.back() == '\n') s.pop_back();
    return s;
}

// GUI + file log
void addLog(string text) {
    int len = GetWindowTextLength(hEditLog);
    SendMessage(hEditLog, EM_SETSEL, len, len);

    wstring w(text.begin(), text.end());
    w += L"\r\n";

    SendMessage(hEditLog, EM_REPLACESEL, 0, (LPARAM)w.c_str());

    ofstream f(logFile, ios::app);
    if (f) f << text << endl;
}

// PROCESS CHECK
void checkProcesses() {

    set<DWORD> currentPIDs;

    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(pe);

    if (Process32First(snap, &pe)) {
        do {
            DWORD pid = pe.th32ProcessID;
            currentPIDs.insert(pid);

            // Unicode fix
            wstring ws(pe.szExeFile);
            string name(ws.begin(), ws.end());

            // START
            if (prevPIDs.find(pid) == prevPIDs.end()) {

                string msg = "PROCESS STARTED: " + name + " | " + getTimeStr();
                addLog(msg);

                startTimes[pid] = time(0);
                processNames[pid] = name;
            }

        } while (Process32Next(snap, &pe));
    }

    CloseHandle(snap);

    // END
    for (DWORD pid : prevPIDs) {

        if (currentPIDs.find(pid) == currentPIDs.end()) {

            time_t end = time(0);
            time_t start = startTimes[pid];

            int duration = (int)difftime(end, start);

            string name = processNames[pid];

            string msg = "PROCESS ENDED: " + name +
                " | " + getTimeStr() +
                " (Duration: " + to_string(duration) + " sec)";

            addLog(msg);
        }
    }

    prevPIDs = currentPIDs;
}

// WINDOW PROC
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {

    switch (msg) {

    case WM_CREATE:

        hBtnStart = CreateWindow(L"BUTTON", L"Start Monitoring",
            WS_VISIBLE | WS_CHILD,
            10, 10, 150, 30,
            hWnd, (HMENU)ID_BTN_START, NULL, NULL);

        hBtnStop = CreateWindow(L"BUTTON", L"Stop Monitoring",
            WS_VISIBLE | WS_CHILD,
            170, 10, 150, 30,
            hWnd, (HMENU)ID_BTN_STOP, NULL, NULL);

        hEditLog = CreateWindow(L"EDIT", L"",
            WS_VISIBLE | WS_CHILD | WS_VSCROLL |
            ES_MULTILINE | ES_READONLY | WS_BORDER,
            10, 50, 560, 300,
            hWnd, (HMENU)ID_EDIT_LOG, NULL, NULL);

        break;

    case WM_COMMAND:

        switch (LOWORD(wp)) {

        case ID_BTN_START:

            prevPIDs.clear();
            startTimes.clear();
            processNames.clear();

            monitoring = true;
            SetTimer(hWnd, ID_TIMER, 2000, NULL);

            addLog("=== Monitoring Started ===");
            break;

        case ID_BTN_STOP:

            monitoring = false;
            KillTimer(hWnd, ID_TIMER);

            addLog("=== Monitoring Stopped ===");
            break;
        }
        break;

    case WM_TIMER:

        if (wp == ID_TIMER && monitoring) {
            checkProcesses();
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, msg, wp, lp);
}

// GUI ENTRY
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR, int nCmdShow) {

    const wchar_t CLASS[] = L"MonitorApp";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = CLASS;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hWnd = CreateWindow(CLASS,
        L"Process Monitor (Start + End + Duration)",
        WS_OVERLAPPEDWINDOW,
        100, 100, 600, 420,
        NULL, NULL, hInst, NULL);

    ShowWindow(hWnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// 🔥 FIX FOR WinMain ERROR (IMPORTANT)
int main() {
    return wWinMain(GetModuleHandle(NULL), NULL, GetCommandLineW(), SW_SHOW);
}