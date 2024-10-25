#include "handler.h"

Handler::Handler(const std::string& comPort, unsigned int baudRate, Logger* logger, bool debug)
    : comPort(comPort), baudRate(baudRate), logger(logger), debug(debug) {
    // Se puede optar por abrir el puerto aquí, si se desea iniciar automáticamente
    if (!serialPort.openDevice(comPort.c_str(), baudRate)) {
        if (logger) {
            logger->Log("Error al intentar abrir el puerto en el constructor.", Logger::ERROR_LOG);
        }
    }
}

// Método para habilitar o deshabilitar el modo de depuración
bool Handler::Debug() const {
    return debug;
}

void Handler::Debug(bool value) {
    debug = value;
}

// Método para abrir el puerto serie
bool Handler::Start() {
    if (serialPort.isDeviceOpen()) {
        logger->Log("El puerto ya está abierto.", Logger::DEBUG);
        return true;
    }

    if (serialPort.openDevice(comPort.c_str(), baudRate) == 0) {
        logger->Log("Conexión establecida con Arduino en el puerto.", Logger::INFO);
        return true;
    }
    else {
        logger->Log("Error al intentar abrir el puerto.", Logger::ERROR_LOG);
        return false;
    }
}

// Método para leer datos del Arduino
std::string Handler::ReadFromArduino() {
    if (!serialPort.isDeviceOpen()) {
        logger->Log("El puerto no está abierto.", Logger::DEBUG);
        return "";
    }

    std::string data;
    char currentChar;
    int result;

    for (int i = 0; i < 180; ++i) {
        result = serialPort.readChar(&currentChar);
        if (result < 0) {
            logger->Log("Error al leer del Arduino.", Logger::ERROR_LOG);
            return "";
        }

        data += currentChar;

        if (currentChar == '.') {
            break;
        }
    }
    if (data.back() == '.') {
        data.pop_back();
    }
    data.erase(std::remove_if(data.begin(), data.end(), [](char c) {
        return c == '\n' || c == '\r';
        }), data.end());
    logger->Log("Datos de Arduino: " + data, Logger::DEBUG);
    return data;
}



// Método para cerrar el puerto serie
void Handler::Stop() {
    if (serialPort.isDeviceOpen()) {
        serialPort.closeDevice();
        logger->Log("Conexión cerrada con Arduino.", Logger::INFO);
    }
}

// Método para enviar un comando al Arduino
void Handler::SendCommand(const std::string& command) {
    if (!serialPort.isDeviceOpen()) {
        logger->Log("El puerto no está abierto.", Logger::DEBUG);
        return;
    }

    int result = serialPort.writeString(command.c_str());
    if (result >= 0) {
        logger->Log("Comando enviado a Arduino: " + command, Logger::DEBUG);
    }
    else {
        logger->Log("Error al enviar comando a Arduino.", Logger::ERROR_LOG);
    }
}

