#include "serial.h"

// Constructor
Serial::Serial() {
    currentStateRTS = true;
    currentStateDTR = true;
    hSerial = INVALID_HANDLE_VALUE;
}

// Destructor
Serial::~Serial() {
    closeDevice();
}

// Función para abrir el dispositivo
char Serial::openDevice(const char* device, const unsigned int baudRate,
    SerialDataBits dataBits,
    SerialParity parity,
    SerialStopBits stopBits) {
    hSerial = CreateFileA(device, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
    if (hSerial == INVALID_HANDLE_VALUE) {
        return (GetLastError() == ERROR_FILE_NOT_FOUND) ? -1 : -2;
    }

    DCB dcbSerialParams;
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) return -3;

    // Configuración de la velocidad de baudios
    switch (baudRate) {
    case 110: dcbSerialParams.BaudRate = CBR_110; break;
    case 300: dcbSerialParams.BaudRate = CBR_300; break;
    case 600: dcbSerialParams.BaudRate = CBR_600; break;
    case 1200: dcbSerialParams.BaudRate = CBR_1200; break;
    case 2400: dcbSerialParams.BaudRate = CBR_2400; break;
    case 4800: dcbSerialParams.BaudRate = CBR_4800; break;
    case 9600: dcbSerialParams.BaudRate = CBR_9600; break;
    case 14400: dcbSerialParams.BaudRate = CBR_14400; break;
    case 19200: dcbSerialParams.BaudRate = CBR_19200; break;
    case 38400: dcbSerialParams.BaudRate = CBR_38400; break;
    case 56000: dcbSerialParams.BaudRate = CBR_56000; break;
    case 57600: dcbSerialParams.BaudRate = CBR_57600; break;
    case 115200: dcbSerialParams.BaudRate = CBR_115200; break;
    case 128000: dcbSerialParams.BaudRate = CBR_128000; break;
    case 256000: dcbSerialParams.BaudRate = CBR_256000; break;
    default: return -4;
    }

    // Configuración de los bits de datos
    BYTE byteSize = 0;
    switch (dataBits) {
    case SERIAL_DATABITS_5: byteSize = 5; break;
    case SERIAL_DATABITS_6: byteSize = 6; break;
    case SERIAL_DATABITS_7: byteSize = 7; break;
    case SERIAL_DATABITS_8: byteSize = 8; break;
    case SERIAL_DATABITS_16: byteSize = 16; break;
    default: return -7;
    }

    // Configuración de los bits de parada
    BYTE stopBitsValue = 0; // Cambia el nombre de la variable para evitar confusiones
    switch (stopBits) {
    case SERIAL_STOPBITS_1: stopBitsValue = ONESTOPBIT; break;
    case SERIAL_STOPBITS_1_5: stopBitsValue = ONE5STOPBITS; break;
    case SERIAL_STOPBITS_2: stopBitsValue = TWOSTOPBITS; break;
    default: return -8;
    }

    // Configuración de la paridad
    BYTE parityValue = 0; // Cambia el nombre de la variable para evitar confusiones
    switch (parity) {
    case SERIAL_PARITY_NONE: parityValue = NOPARITY; break;
    case SERIAL_PARITY_EVEN: parityValue = EVENPARITY; break;
    case SERIAL_PARITY_ODD: parityValue = ODDPARITY; break;
    case SERIAL_PARITY_MARK: parityValue = MARKPARITY; break;
    case SERIAL_PARITY_SPACE: parityValue = SPACEPARITY; break;
    default: return -9;
    }

    // Asignar configuraciones a dcbSerialParams
    dcbSerialParams.ByteSize = byteSize;
    dcbSerialParams.StopBits = stopBitsValue;
    dcbSerialParams.Parity = parityValue;

    // Aplicar configuraciones
    if (!SetCommState(hSerial, &dcbSerialParams)) return -5;

    // Inicialización de timeouts
    COMMTIMEOUTS timeouts;
    memset(&timeouts, 0, sizeof(timeouts)); // Inicializa la estructura
    timeouts.ReadIntervalTimeout = 50; // Ejemplo, establece un valor predeterminado
    timeouts.ReadTotalTimeoutConstant = MAXDWORD;
    timeouts.ReadTotalTimeoutMultiplier = 10; // Ejemplo, establece un valor predeterminado
    timeouts.WriteTotalTimeoutConstant = MAXDWORD;
    timeouts.WriteTotalTimeoutMultiplier = 10; // Ejemplo, establece un valor predeterminado

    // Aplicar timeouts
    if (!SetCommTimeouts(hSerial, &timeouts)) return -6;

    return 1;
}

// Comprobar si el dispositivo está abierto
bool Serial::isDeviceOpen() {
    return hSerial != INVALID_HANDLE_VALUE;
}

// Cerrar el dispositivo
void Serial::closeDevice() {
    CloseHandle(hSerial);
    hSerial = INVALID_HANDLE_VALUE;
}

// Escribir un carácter
int Serial::writeChar(const char byte) {
    DWORD dwBytesWritten;
    if (!WriteFile(hSerial, &byte, 1, &dwBytesWritten, NULL)) return -1;
    return 1;
}

// Escribir una cadena
int Serial::writeString(const char* receivedString) {
    DWORD dwBytesWritten;
    if (!WriteFile(hSerial, receivedString, strlen(receivedString), &dwBytesWritten, NULL))
        return -1;
    return 1;
}

// Escribir bytes
int Serial::writeBytes(const void* buffer, const unsigned int nbBytes) {
    DWORD dwBytesWritten;
    if (!WriteFile(hSerial, buffer, nbBytes, &dwBytesWritten, NULL))
        return -1;
    return 1;
}

// Leer un carácter
int Serial::readChar(char* pByte, unsigned int timeOutMs) {
    DWORD dwBytesRead = 0;

    timeouts.ReadTotalTimeoutConstant = timeOutMs;

    if (!SetCommTimeouts(hSerial, &timeouts)) return -1;

    if (!ReadFile(hSerial, pByte, 1, &dwBytesRead, NULL)) return -2;

    if (dwBytesRead == 0) return 0;

    return 1;
}

// Leer una cadena sin tiempo de espera
int Serial::readStringNoTimeOut(char* receivedString, char finalChar, unsigned int maxNbBytes) {
    unsigned int NbBytes = 0;
    char charRead;

    while (NbBytes < maxNbBytes) {
        charRead = readChar(&receivedString[NbBytes]);

        if (charRead == 1) {
            if (receivedString[NbBytes] == finalChar) {
                receivedString[++NbBytes] = 0;
                return NbBytes;
            }

            NbBytes++;
        }

        if (charRead < 0) return charRead;
    }
    return -3;
}

// Leer una cadena con tiempo de espera
int Serial::readString(char* receivedString, char finalChar, unsigned int maxNbBytes, unsigned int timeOutMs) {
    if (timeOutMs == 0) return readStringNoTimeOut(receivedString, finalChar, maxNbBytes);

    unsigned int nbBytes = 0;
    char charRead;
    Timer timer;
    long int timeOutParam;

    timer.initTimer();

    while (nbBytes < maxNbBytes) {
        timeOutParam = timeOutMs - timer.elapsedTime_ms();

        if (timeOutParam > 0) {
            charRead = readChar(&receivedString[nbBytes], timeOutParam);

            if (charRead == 1) {
                if (receivedString[nbBytes] == finalChar) {
                    receivedString[++nbBytes] = 0;
                    return nbBytes;
                }
                nbBytes++;
            }
            if (charRead < 0) return charRead;
        }
        if (timer.elapsedTime_ms() > timeOutMs) {
            receivedString[nbBytes] = 0;
            return 0;
        }
    }

    return -3;
}

// Leer bytes
int Serial::readBytes(void* buffer, unsigned int maxNbBytes, unsigned int timeOutMs, unsigned int sleepDurationUs) {
    UNUSED(sleepDurationUs);

    DWORD dwBytesRead = 0;

    timeouts.ReadTotalTimeoutConstant = (DWORD)timeOutMs;

    if (!SetCommTimeouts(hSerial, &timeouts)) return -1;

    if (!ReadFile(hSerial, buffer, (DWORD)maxNbBytes, &dwBytesRead, NULL)) return -2;

    return dwBytesRead;
}

// Limpiar el receptor
char Serial::flushReceiver() {
    return PurgeComm(hSerial, PURGE_RXCLEAR);
}

// Comprobar disponibilidad de datos
int Serial::available() {
    DWORD commErrors;
    COMSTAT commStatus;
    ClearCommError(hSerial, &commErrors, &commStatus);
    return commStatus.cbInQue;
}
