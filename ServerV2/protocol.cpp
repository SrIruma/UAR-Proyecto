#include "protocol.h"
#pragma comment(lib, "Ws2_32.lib")

Protocol::Protocol(const std::string& host, int port, Handler* arduinoHandler, int maxConnections, Logger* logger, bool debug)
    : arduinoHandler(arduinoHandler), maxConnections(maxConnections), logger(logger), debug(debug), isRunning(false) {

    this->port = std::to_string(port);

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        logger->Log("Error al iniciar Winsock.", Logger::ERROR_LOG);
        return;
    }

    // Configura el socket del servidor
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        logger->Log("Error al crear el socket del servidor.", Logger::ERROR_LOG);
        WSACleanup();
        return;
    }

    servAddr.sin_family = AF_INET;

    // Cambia inet_addr por inet_pton
    if (inet_pton(AF_INET, host.c_str(), &servAddr.sin_addr) <= 0) {
        logger->Log("Error al convertir la dirección IP.", Logger::ERROR_LOG);
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    servAddr.sin_port = htons(port);

    // Vincula el socket
    if (bind(serverSocket, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
        logger->Log("Error al vincular el socket del servidor.", Logger::ERROR_LOG);
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    // Escucha conexiones entrantes
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        logger->Log("Error al iniciar la escucha en el socket del servidor.", Logger::ERROR_LOG);
        closesocket(serverSocket);
        WSACleanup();
        return;
    }
}

bool Protocol::Debug() const {
    return debug;
}

void Protocol::Debug(bool value) {
    debug = value;
}

bool Protocol::Start() {
    if (isRunning) {
        logger->Log("El servidor ya está en ejecución.", Logger::WARNING);
        return false;
    }

    isRunning = true;

    if (!arduinoHandler->Start()) {
        logger->Log("No se pudo establecer conexión con el Arduino. El servidor no se iniciará.", Logger::ERROR_LOG);
        isRunning = false;
        return false;
    }

    logger->Log("Servidor TCP ejecutandose en " + color::BRIGHT_YELLOW + GetLocalIPAddress() + ":" +
        port + color::RESET + ", esperando conexiones...", Logger::INFO);

    // Inicia hilos para aceptar clientes y leer datos de Arduino
    std::thread(&Protocol::AcceptClients, this).detach();
    std::thread(&Protocol::ReadAndBroadcastArduinoData, this).detach();

    return true;
}

void Protocol::Stop() {
    isRunning = false;

    for (SOCKET client : clients) {
        closesocket(client);
    }
    arduinoHandler->Stop();
    closesocket(serverSocket);
    WSACleanup();
    logger->Log("Servidor TCP detenido.", Logger::INFO);
}

void Protocol::AcceptClients() {
    while (isRunning) {
        if (clients.size() >= maxConnections) {
            logger->Log("Número máximo de conexiones alcanzado.", Logger::WARNING);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }

        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket != INVALID_SOCKET) {
            clients.push_back(clientSocket);
            logger->Log("Cliente conectado.",Logger::INFO);
            std::thread(&Protocol::HandleClient, this, clientSocket).detach();
        }
        else {
            logger->Log("Error al aceptar la conexión del cliente.", Logger::ERROR_LOG);
        }
    }
}

void Protocol::HandleClient(SOCKET clientSocket) {
    char buffer[1024];
    int bytesRead;

    try {
        while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
            std::string data(buffer, bytesRead);
            logger->Log("[" + color::CYAN + "CLIENT" + color::RESET + "] " + data, Logger::INFO);

            std::string response = "Datos recibidos: ";
            send(clientSocket, response.c_str(), response.length(), 0);
            logger->Log("[CLIENT] Datos enviados.", Logger::DEBUG);
        }
    }
    catch (const std::exception& ex) {
        logger->Log("Error en el cliente: " + std::string(ex.what()), Logger::ERROR_LOG);
    }

    closesocket(clientSocket);
    clients.erase(std::remove(clients.begin(), clients.end(), clientSocket), clients.end());
    logger->Log("Cliente desconectado.",Logger::WARNING);
}

void Protocol::ReadAndBroadcastArduinoData() {
    while (isRunning) {
        std::string arduinoData = arduinoHandler->ReadFromArduino();
        if (!arduinoData.empty()) {
            arduinoData.erase(std::remove(arduinoData.begin(), arduinoData.end(), '.'), arduinoData.end());
            BroadcastToClients(arduinoData + '\n');
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void Protocol::BroadcastToClients(const std::string& message) {
    for (SOCKET client : clients) {
        if (client != INVALID_SOCKET) {
            send(client, message.c_str(), message.length(), 0);
            logger->Log("Mensaje enviado a un cliente: " + message, Logger::DEBUG);
        }
    }
}

std::string Protocol::GetLocalIPAddress() {
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        throw std::runtime_error("No se pudo obtener el nombre del host.");
    }

    addrinfo hints = {};
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;

    addrinfo* result = nullptr;
    int res = getaddrinfo(hostname, nullptr, &hints, &result);
    if (res != 0) {
        throw std::runtime_error("No se pudo obtener la dirección IP válida.");
    }

    std::string ipAddress;
    for (addrinfo* addr = result; addr != nullptr; addr = addr->ai_next) {
        char ipStr[INET_ADDRSTRLEN];
        if (addr->ai_family == AF_INET) {
            // Convierte la dirección IP a una cadena
            sockaddr_in* sockaddr = reinterpret_cast<sockaddr_in*>(addr->ai_addr);
            if (inet_ntop(AF_INET, &sockaddr->sin_addr, ipStr, sizeof(ipStr))) {
                ipAddress = ipStr; // Asigna la dirección IP encontrada
                break; // Sale del bucle después de encontrar la primera dirección IP
            }
        }
    }

    freeaddrinfo(result); // Libera la memoria
    if (ipAddress.empty()) {
        throw std::runtime_error("No se encontró una dirección IP válida.");
    }

    return ipAddress;
}