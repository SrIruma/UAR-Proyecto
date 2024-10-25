#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <conio.h>
#include "Logger.h"
#include "Protocol.h"
#include "Handler.h"

class CommandLineInterface {
public:
    void Start(bool showCommander, const std::vector<std::string>& args = {});

private:
    void ProcessCommand(const std::string& command);
    void RunServer(const std::vector<std::string>& args);
    void PrintDefaults();
    void ShowProjectInfo();
    void PrintCommands();
    void PrintCommander();
    void PrintKeyCommands();
    void ClearConsole();

    void UpdatePort(const std::string& port);
    void UpdateComPort(const std::vector<std::string>& args);
    void UpdateBaudRate(const std::vector<std::string>& args);
    void UpdateMaxConnections(const std::vector<std::string>& args);
    void InitServer();
    void StopServer();

    Logger* logger = nullptr;
    Protocol* protocol = nullptr;
    Handler* handler = nullptr;

    std::string host = "0.0.0.0";
    int port = 25565;
    std::string comPort = "COM3";
    int baudRate = 9600;
    int maxConnections = 5;
    bool debugMode = false;
    bool isRunning = false;
};