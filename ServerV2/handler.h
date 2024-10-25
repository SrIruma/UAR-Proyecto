#pragma once

#include <string>
#include "serial.h"
#include "logger.h"

/// <summary>
/// Clase que maneja la comunicaci�n con un dispositivo Arduino a trav�s de un puerto serie.
/// </summary>
class Handler {
public:
    /**
     * @brief Constructor de la clase Handler.
     * @param comPort Nombre del puerto COM al que est� conectado Arduino.
     * @param baudRate Tasa de baudios para la comunicaci�n.
     * @param logger Instancia del logger para manejar mensajes de log.
     * @param debug Indica si se activa el modo de depuraci�n (por defecto es false).
     */
    Handler(const std::string& comPort, unsigned int baudRate, Logger* logger, bool debug = false);

    /**
     * @brief Propiedad para habilitar o deshabilitar el modo de depuraci�n.
     */
    bool Debug() const;
    void Debug(bool value);

    /**
     * @brief Abre el puerto serie para iniciar la comunicaci�n con Arduino.
     * @return true si se pudo abrir, false en caso contrario.
     */
    bool Start();

    /**
     * @brief Lee datos del Arduino hasta que se encuentra el terminador '.'.
     * @return Una cadena que contiene los datos le�dos, sin el terminador '.' al final.
     */
    std::string ReadFromArduino();

    /**
     * @brief Cierra el puerto serie, finalizando la comunicaci�n con Arduino.
     */
    void Stop();

    /**
     * @brief Env�a un comando al Arduino a trav�s del puerto serie.
     * @param command El comando que se desea enviar a Arduino.
     */
    void SendCommand(const std::string& command);

private:
    Serial serialPort; ///< Objeto que representa el puerto serie para la comunicaci�n.
    Logger* logger;    ///< Instancia del logger para manejar mensajes de log.
    bool debug;        ///< Indica si se debe activar el modo de depuraci�n.
    std::string comPort; ///< Nombre del puerto COM.
    unsigned int baudRate; ///< Tasa de baudios.
};
