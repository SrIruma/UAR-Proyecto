#pragma once

#include <string>
#include <iostream>
#include "color.h"

class Logger {
public:
    // Enumeración para los niveles de severidad
    enum Severity {
        WARNING,
        ERROR_LOG,
        DEBUG,
        INFO,
        UNKNOWN
    };

    /**
     * @brief Constructor de Logger.
     * @param debug Indica si el modo de depuración debe estar activado (por defecto es false).
     */
    Logger(bool debug = false);

    /**
     * @brief Registra un mensaje con un nivel de severidad.
     * @param severity El nivel de severidad del mensaje.
     * @param message El mensaje a registrar.
     */
    void Log(const std::string& message, Severity severity);

    /**
     * @brief Propiedad para habilitar o deshabilitar el modo de depuración.
     */
    bool Debug() const;
    void Debug(bool value);

private:
    bool debug; ///< Indica si el modo de depuración está activado.

    /**
     * @brief Escribe el mensaje de registro formateado.
     * @param severity El nivel de severidad del mensaje.
     * @param message El mensaje a registrar.
     */
    void WriteLog(Severity severity, const std::string& message);
};