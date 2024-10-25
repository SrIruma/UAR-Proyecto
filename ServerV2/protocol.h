#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include "handler.h"
#include "logger.h"

class Protocol {
public:
    Protocol(const std::string& host, int port, Handler* arduinoHandler, int maxConnections, Logger* logger, bool debug);
    bool Start();
    void Stop();

    bool Debug() const;
    void Debug(bool value);

private:
    void AcceptClients();
    void HandleClient(SOCKET clientSocket);
    void ReadAndBroadcastArduinoData();
    void BroadcastToClients(const std::string& message);
    std::string GetLocalIPAddress();

    SOCKET serverSocket;
    Handler* arduinoHandler;
    bool isRunning;
    std::vector<SOCKET> clients;
    int maxConnections;
    std::string port;
    Logger* logger;
    bool debug;
    sockaddr_in servAddr; 
};