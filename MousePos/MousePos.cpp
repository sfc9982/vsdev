#include <chrono>
#include <conio.h>
#include <cstdio>
#include <iostream>
#include <thread>
#include <windows.h>

DWORD WINAPI MonitorMousePosition(LPVOID lpParam);

int __cdecl main() {
    if (IsDebuggerPresent()) {
        exit(EXIT_FAILURE);
    }

    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);

    HANDLE hThread;
    DWORD  dwThreadId;

    hThread = CreateThread(
            nullptr,
            0,
            MonitorMousePosition,
            nullptr,
            0,
            &dwThreadId);

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    return 0;
}

DWORD WINAPI MonitorMousePosition(LPVOID lpParam) {
    POINT pos;
    POINT lastPos;
    GetCursorPos(&lastPos);

    while (true) {
        if (_kbhit()) {
            int ret = _getch();
            if ('q' == ret) {
                ExitThread(0);
            }
        }

        if (GetCursorPos(&pos); pos.x != lastPos.x || pos.y != lastPos.y) {
            printf("(%ld,%ld)\n", pos.x + 1, pos.y + 1);
            lastPos = pos;
        }

        // std::this_thread::sleep_for(std::chrono::duration(std::chrono::milliseconds(1)));
        Sleep(1);
    }
}