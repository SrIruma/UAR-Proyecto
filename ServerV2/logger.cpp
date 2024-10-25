#include "logger.h"

Logger::Logger(bool debug) : debug(debug) {}

bool Logger::Debug() const {
    return debug;
}

void Logger::Debug(bool value) {
    debug = value;
}

void Logger::Log(const std::string& message, Severity severity) {
    WriteLog(severity, message);
}

void Logger::WriteLog(Severity severity, const std::string& message) {
    std::string severityStr;
    std::string colorStr;

    switch (severity) {
    case Logger::WARNING:
        severityStr = "WARNING";
        colorStr = color::yellow(severityStr, true);
        break;
    case Logger::ERROR_LOG:
        severityStr = "ERROR";
        colorStr = color::red(severityStr, true);
        break;
    case Logger::DEBUG:
        if (!debug) return;
        severityStr = "DEBUG";
        colorStr = color::green(severityStr, true);
        break;
    case Logger::INFO:
        severityStr = "INFO";
        colorStr = color::cyan(severityStr, true);
        break;
    case Logger::UNKNOWN:
    default:
        severityStr = "UNKNOWN";
        colorStr = color::white(severityStr, true);
        break;
    }

    std::cout << color::white(" [")
        << colorStr
        << color::white("] ")
        << color::RESET << message << std::endl;
}