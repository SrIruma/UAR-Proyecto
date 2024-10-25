#include "CommandLineInterface.h"
#include <iostream>
#include <Windows.h>
#include <vector>
#include "resource.h"

void SetConsoleProperties() {
    const int width = 805;
    const int height = 500;

    HWND consoleWindow = GetConsoleWindow();
    MoveWindow(consoleWindow, 255, 123, width, height, TRUE);

    SetConsoleTitleA("ULTRASONIC RADAR SERVER PROYECT - UCENM - TERMINAL");

    HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_RADAR_ICON));
    SendMessage(consoleWindow, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    SendMessage(consoleWindow, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

    LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
    style &= ~WS_MAXIMIZEBOX;
    style &= ~WS_SIZEBOX;
    SetWindowLong(consoleWindow, GWL_STYLE, style);
    SetWindowPos(consoleWindow, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "");

    SetConsoleProperties();

    CommandLineInterface cli;
    std::vector<std::string> args;

    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }

    cli.Start(true, args);

    return 0;
}
