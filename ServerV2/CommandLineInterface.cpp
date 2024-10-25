#include "CommandLineInterface.h"

void CommandLineInterface::Start(bool showCommander, const std::vector<std::string>& args) {
    if (!args.empty()) {
        for (size_t i = 0; i < args.size(); ++i) {
            const std::string& cmd = args[i];
            std::string commandToProcess = cmd;

            if (i + 1 < args.size()) {
                const std::string& firstArg = args[i + 1];
                commandToProcess += " " + firstArg;

                if (i + 2 < args.size() && args[i + 2].rfind("--", 0) == 0) {
                    commandToProcess += " " + args[i + 2];
                    i += 2;
                }
                else {
                    i++;
                }
                ProcessCommand(commandToProcess);
            }
            else {
                std::cerr << "[ERROR] Se requiere un argumento después de " << cmd << "." << std::endl;
            }
        }
    }
    std::string command;
    ShowProjectInfo();
    while (true) {
        if (showCommander) {
            PrintCommander();
        }
        std::getline(std::cin, command);
        if (command.empty()) {
            continue;
        }
        ProcessCommand(command);
    }
}

void CommandLineInterface::ProcessCommand(const std::string& command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == "exit" || cmd == "-e") {
        StopServer();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        exit(1);
        return;
    }
    else if (cmd == "start" || cmd == "-s") {
        std::string process;
        iss >> process;  

        std::vector<std::string> params; 
        std::string param;

        while (iss >> param) {
            params.push_back(param);
        }

        std::string sys = "start " + process;
        for (const auto& p : params) {
            sys += " " + p;
        }

        std::cout << "Ejecutando: " << sys << std::endl;

        system(sys.c_str());
    }

    else if (cmd == "info" || cmd == "-i") {
        ShowProjectInfo();
    }
    else if (cmd == "config" || cmd == "-cf") {
        PrintDefaults();
    }
    else if (cmd == "help" || cmd == "-h") {
        PrintCommands();
    }
    else if (cmd == "clear" || cmd == "-cl") {
        ClearConsole();
    }
    else if (cmd == "port" || cmd == "-p") {
        std::string port;
        iss >> port;
        UpdatePort(port);
    }
    else if (cmd == "com-port" || cmd == "-c") {
        std::string comPort;
        iss >> comPort;
        UpdateComPort({ cmd, comPort });
    }
    else if (cmd == "baudrate" || cmd == "-b") {
        std::string baudRate;
        iss >> baudRate;
        UpdateBaudRate({ cmd, baudRate });
    }
    else if (cmd == "max-cons" || cmd == "-m") {
        std::string maxConnections;
        iss >> maxConnections;
        std::string forceArg;
        if (iss >> forceArg) {
            UpdateMaxConnections({ cmd, maxConnections, forceArg });
        }
        else {
            UpdateMaxConnections({ cmd, maxConnections });
        }
    }
    else if (cmd == "run" || cmd == "-r") {
        std::vector<std::string> args;
        std::string arg;
        while (iss >> arg) {
            args.push_back(arg);
        }
        RunServer({ cmd, arg });
    }
    else if (cmd == ";" || cmd == ".") {
        std::cout << "\e[A";
    }
    else {
        logger->Log("Comando desconocido. Usa" + color::BRIGHT_RED + " help " + color::RESET + "para ver las opciones.", Logger::WARNING);
    }
}

void CommandLineInterface::PrintCommander() {
    std::cout << color::BOLD + color::BRIGHT_GREEN + "UCENM@ADMIN" + color::BRIGHT_YELLOW + ":~$ " + color::RESET << std::ends;
}

void CommandLineInterface::PrintDefaults() {
    std::string serverState = (isRunning ? "En Ejecución" : "Inactivo");
    std::string debugState = (debugMode ? "Activado" : "Apagado");
    std::vector<std::string> defsInfo = {
        "\n------------------------------------------------------------------------------------------------",
        "                           CONFIGURACIÓN ACTUAL DE INICIALIZACIÓN TCP",
        "------------------------------------------------------------------------------------------------",
        " HOST                    : " + host + " (Dirección Universal Local)",
        " PUERTO TCP              : " + std::to_string(port),
        " PUERTO ARDUINO          : " + comPort,
        " BAUD RATE               : " + std::to_string(baudRate),
        " MÁXIMO DE CONEXIONES    : " + std::to_string(maxConnections) + " (Clientes)",
        " ESTADO DEL SERVIDOR     : " + serverState,
        " MODO DEPURACIÓN         : " + debugState,
        "------------------------------------------------------------------------------------------------\n",
    };

    for (const auto& line : defsInfo) {
        std::cout << line << std::endl;
    }
}

void CommandLineInterface::ShowProjectInfo() {
    std::vector<std::string> projectInfo = {
    "\n------------------------------------------------------------------------------------------------",
    color::BRIGHT_BLUE+" PROYECTO SERVIDOR ARDUINO | PROTOCOLO TCP/IP - HANDLER - CLIENTE/SERVIDOR"+color::RESET,
    "------------------------------------------------------------------------------------------------",
    " Nombre del Proyecto   : Servidor de Radar Ultrasónico UAR-S®",
    " Versión               : v1.1.2 (build 439)",
    " Fecha de Lanzamiento  : Octubre 2024",
    " Desarrollado Por      : Estudiantes de Ingeniería en Sistemas - UCENM",
    "------------------------------------------------------------------------------------------------",
    " Descripción           : Este proyecto utiliza un radar ultrasónico para detectar objetos, ",
    "                       : enviando datos en tiempo real a una aplicación móvil. El servidor ",
    "                       : está desarrollado en C++ y gestiona las conexiones entre Arduino y ",
    "                       : los dispositivos móviles.",
    "------------------------------------------------------------------------------------------------",
    " Características       : - Comunicación Cliente/Servidor bidireccional por TCP/IP",
    "                       : - Envío de datos serial en tiempo real",
    "                       : - Configuración ajustable del servidor",
    "------------------------------------------------------------------------------------------------",
    " ¿Necesitas ayuda?     : Usa el comando " + color::BRIGHT_RED + "help" + color::RESET + " o " + color::BRIGHT_RED + "-h" + color::RESET + " para abrir el menú de ayuda.",
    "------------------------------------------------------------------------------------------------\n",
    };

    for (const auto& line : projectInfo) {
        std::cout << line << std::endl;
    }
}

void CommandLineInterface::PrintCommands() {
    std::vector<std::string> commandsInfo = {
    "\n------------------------------------------------------------------------------------------------",
    " COMANDOS      | PARAMETRO       | DESCRIPCIÓN       ",
    "------------------------------------------------------------------------------------------------",
    " -cl, clear                      : Limpia la terminal.",
    " -cf, config                     : Muestra la configuración establecida del servidor.",
    " -h,  help                       : Muestra este menú de ayuda.",
    " -i,  info                       : Muestra más información de este programa.",
    " -p,  port      [puerto]         : Establece el puerto del servidor.",
    " -c,  com-port  [COM]            : Establece el puerto serial de Arduino.",
    " -b,  baudrate  [baud]           : Establece la tasa de baudios.",
    " -m,  max-cons  [1-10] [--f]     : Establece el número máximo de conexiones.",
    " -r,  run       [--debug]        : Enciende el servidor TCP y abre las conexiones con Arduino.",
    " -s,  start     [cmd] [args]     : Ejecuta un proceso externo con argumentos opcionales.",
    " -e,  exit                       : Cierra el servidor y por ende el programa.",
    "------------------------------------------------------------------------------------------------\n",
    };

    for (const auto& line : commandsInfo) {
        std::cout << line << std::endl;
    }
}

void CommandLineInterface::PrintKeyCommands() {

    std::vector<std::string> keysInfo = {
    "\n------------------------------------------------------------------------------------------------",
    " Tecla             | Descripción",
    "------------------------------------------------------------------------------------------------",
    "  F8               : Limpia la consola.",
    "  F9               : Alterna el modo de depuración (On/Off).",
    "  F10              : Detiene el servidor y cierra todas las conexiones.",
    "------------------------------------------------------------------------------------------------\n",
    };

    for (const auto& line : keysInfo) {
        std::cout << line << std::endl;
    }
}

void CommandLineInterface::UpdatePort(const std::string& arg) {
    const std::vector<int> ReservedTcpPorts = {
    0, 7, 9, 13, 17, 19, 20, 21, 22, 23, 25, 53, 67, 68, 69, 80, 110, 135, 137, 138, 139,
    143, 161, 162, 389, 443, 445, 464, 514, 515, 520, 587, 631, 993, 995
    };
    try {
        int port = std::stoi(arg);

        if (std::find(ReservedTcpPorts.begin(), ReservedTcpPorts.end(), port) != ReservedTcpPorts.end()) {
            logger->Log("El puerto " + std::to_string(port) + " está reservado y no se puede usar.", Logger::ERROR_LOG);
            return;
        }

        if (port < 1024 || port > 65535) {
            throw std::out_of_range("El puerto debe estar entre 1024 y 65535");
        }

        if (this->port == port) {
            logger->Log("El puerto " + std::to_string(port) + " ya está asignado.", Logger::WARNING);
            return;
        }

        this->port = port;
        logger->Log("Puerto configurado: " + std::to_string(port), Logger::INFO);
    }
    catch (const std::invalid_argument&) {
        logger->Log("Número de puerto inválido: " + arg, Logger::ERROR_LOG);
    }
    catch (const std::out_of_range&) {
        logger->Log("Número de puerto fuera de rango: " + arg, Logger::ERROR_LOG);
    }
}

void CommandLineInterface::UpdateComPort(const std::vector<std::string>& args) {
    if (args.size() > 1) {
        std::string comPort = args[1];

        for (char& c : comPort) {
            c = std::toupper(c);
        }

        try {
            if (comPort.substr(0, 3) == "COM" && comPort.size() > 3 && std::isdigit(comPort[3]) &&
                std::stoi(comPort.substr(3)) >= 1 && std::stoi(comPort.substr(3)) <= 255) {

                if (this->comPort == comPort) {
                    logger->Log("El puerto COM " + comPort + " ya está asignado.", Logger::WARNING);
                    return;
                }

                this->comPort = comPort;
                logger->Log("Puerto COM configurado: " + this->comPort, Logger::INFO);
            }
            else {
                logger->Log("Debes especificar un puerto COM válido (ej. COM1, COM2, ... COM255).", Logger::ERROR_LOG);
            }
        }
        catch (const std::invalid_argument&) {
            logger->Log("Puerto COM inválido: " + comPort, Logger::ERROR_LOG);
        }
    }
    else {
        logger->Log("Debes especificar un puerto COM.", Logger::ERROR_LOG);
    }
}

void CommandLineInterface::UpdateBaudRate(const std::vector<std::string>& args) {
    const std::vector<int> commonBaudRates = { 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 38400, 57600, 115200 };

    if (args.size() > 1) {
        try {
            int baudrate = std::stoi(args[1]);

            // Verificar que el baud rate sea uno de los comunes
            if (std::find(commonBaudRates.begin(), commonBaudRates.end(), baudrate) != commonBaudRates.end()) {
                if (this->baudRate == baudrate) {
                    logger->Log("La tasa de baudios " + std::to_string(baudrate) + " ya está asignada.", Logger::WARNING);
                    return;
                }

                this->baudRate = baudrate;
                logger->Log("Tasa de baudios configurada: " + std::to_string(this->baudRate), Logger::INFO);
            }
            else {
                logger->Log("Tasa de baudios no válida para Arduino: " + std::to_string(baudrate), Logger::ERROR_LOG);
            }
        }
        catch (const std::invalid_argument&) {
            logger->Log("Debes especificar una tasa de baudios válida.", Logger::ERROR_LOG);
        }
    }
    else {
        logger->Log("Debes especificar una tasa de baudios.", Logger::ERROR_LOG);
    }
}

void CommandLineInterface::UpdateMaxConnections(const std::vector<std::string>& args) {
    bool force = false;

    if (args.size() > 2 && args[2] == "--f") {
        force = true;
    }
    if (args.size() > 1) {
        try {
            int maxConnections = std::stoi(args[1]);

            if (!force) {
                if (maxConnections < 1 || maxConnections > 10) {
                    logger->Log("Debes especificar un número de conexiones válido (1-10).", Logger::ERROR_LOG);
                    return;
                }
            }

            if (this->maxConnections == maxConnections) {
                logger->Log("El número máximo de conexiones " + std::to_string(maxConnections) + " ya está asignado.", Logger::WARNING);
                return;
            }

            this->maxConnections = maxConnections;
            if (force) {
                logger->Log("Número máximo de conexiones forzado a: " + std::to_string(this->maxConnections), Logger::WARNING);
            }
            else{
                logger->Log("Número máximo de conexiones configurado: " + std::to_string(this->maxConnections), Logger::INFO);
            }
        }
        catch (const std::invalid_argument&) {
            logger->Log("Número máximo de conexiones inválido: " + args[1], Logger::ERROR_LOG);
        }
    }
    else {
        logger->Log("Debes especificar un número de conexiones.", Logger::ERROR_LOG);
    }
}

void CommandLineInterface::ClearConsole() {
    std::system("cls");
}

void CommandLineInterface::RunServer(const std::vector<std::string>& args) {
    if (args.size() > 1 && args[1] == "--debug") {
        debugMode = true;
    }
    else {
        debugMode = false;
    }

    std::string debugState = (debugMode ? "Activado..." : "Desactivado...");
    logger->Log("El Modo depuración a sido " + debugState, Logger::WARNING);
    InitServer();
}

void CommandLineInterface::InitServer() {
    logger = new Logger(debugMode);
    handler = new Handler(comPort, baudRate, logger, debugMode);
    protocol = new Protocol(host, port, handler, maxConnections, logger, debugMode);

    if (protocol->Start()) {
        isRunning = true;
        PrintKeyCommands();

        while (isRunning) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            if (GetAsyncKeyState(VK_F8) & 0x8000) {
                ProcessCommand("-cl");
            }

            if (GetAsyncKeyState(VK_F9) & 0x8000) {
                debugMode = !debugMode;
                logger->Debug(debugMode);
                handler->Debug(debugMode);
                protocol->Debug(debugMode);
                std::string debugState = (debugMode ? "Activado..." : "Desactivado...");
                logger->Log("El Modo depuración a sido " + debugState, Logger::WARNING);
            }

            if (GetAsyncKeyState(VK_F10) & 0x8000) {
                StopServer();
            }
        }
    }
    else {
        handler = nullptr;
        protocol = nullptr;
        return;
    }
}

void CommandLineInterface::StopServer() {
    if (protocol == nullptr || !isRunning)
    {
        logger->Log("No hay servidores en ejecución para detener.",Logger::INFO);
        return;
    }
    logger->Log("Deteniendo servidor actual, se cerraran todos los clientes...", Logger::WARNING);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    protocol->Stop();
    isRunning = false;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}