#pragma once

#include <windows.h>
#include <iostream>
#include <cstring> // Para memset

#define UNUSED(x) (void)(x)

/**
 * number of serial data bits
 */
enum SerialDataBits {
    SERIAL_DATABITS_5,  /**< 5 databits */
    SERIAL_DATABITS_6,  /**< 6 databits */
    SERIAL_DATABITS_7,  /**< 7 databits */
    SERIAL_DATABITS_8,  /**< 8 databits */
    SERIAL_DATABITS_16, /**< 16 databits */
};

/**
 * number of serial stop bits
 */
enum SerialStopBits {
    SERIAL_STOPBITS_1,    /**< 1 stop bit */
    SERIAL_STOPBITS_1_5,  /**< 1.5 stop bits */
    SERIAL_STOPBITS_2,    /**< 2 stop bits */
};

/**
 * type of serial parity bits
 */
enum SerialParity {
    SERIAL_PARITY_NONE, /**< no parity bit */
    SERIAL_PARITY_EVEN, /**< even parity bit */
    SERIAL_PARITY_ODD,  /**< odd parity bit */
    SERIAL_PARITY_MARK, /**< mark parity */
    SERIAL_PARITY_SPACE /**< space bit */
};

class Timer {
public:
    void initTimer() {
        startTime = GetTickCount();
    }

    unsigned long elapsedTime_ms() {
        return GetTickCount() - startTime;
    }

private:
    unsigned long startTime;
};

/*! \class Serial
 *  \brief This class is used for communication over a serial device.
 */
class Serial {
public:
    // Constructors and destructors
    Serial();
    ~Serial();

    // Configuration and initialization
    char openDevice(const char* device, const unsigned int baudRate,
        SerialDataBits dataBits = SERIAL_DATABITS_8,
        SerialParity parity = SERIAL_PARITY_NONE,
        SerialStopBits stopBits = SERIAL_STOPBITS_1);
    bool isDeviceOpen();
    void closeDevice();

    // Read/Write operation on characters
    int writeChar(char);
    int readChar(char* pByte, const unsigned int timeOutMs = 0);

    // Read/Write operation on strings
    int writeString(const char* string);
    int readString(char* receivedString, char finalChar,
        unsigned int maxNbBytes, const unsigned int timeOutMs = 0);

    // Read/Write operation on bytes
    int writeBytes(const void* buffer, const unsigned int nbBytes);
    int readBytes(void* buffer, unsigned int maxNbBytes,
        const unsigned int timeOutMs = 0, unsigned int sleepDurationUs = 100);

    // Special operation
    char flushReceiver();
    int available();

    // Access to IO bits
    bool DTR(bool status);
    bool setDTR();
    bool clearDTR();
    bool RTS(bool status);
    bool setRTS();
    bool clearRTS();
    bool isRI();
    bool isDCD();
    bool isCTS();
    bool isDSR();
    bool isRTS();
    bool isDTR();

private:
    // Helper function for reading strings without timeout
    int readStringNoTimeOut(char* string, char finalChar, unsigned int maxNbBytes);

    // State variables for RTS and DTR
    bool currentStateRTS;
    bool currentStateDTR;
    Timer timer;
#if defined(_WIN32) || defined(_WIN64)
    HANDLE hSerial; // Handle on serial device
    COMMTIMEOUTS timeouts; // For setting serial port timeouts
#endif
};

