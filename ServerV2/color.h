#pragma once

#include <string>

namespace color {
    // Códigos ANSI para colores
    const std::string RESET = "\033[0m";       ///< Reinicia el color de texto.
    const std::string BOLD = "\033[1m";         ///< Establece el texto en negrita.
    const std::string WHITE = "\033[1;37m";     ///< Color blanco.
    const std::string YELLOW = "\033[1;33m";    ///< Color amarillo.
    const std::string GREEN = "\033[0;32m";     ///< Color verde.
    const std::string RED = "\033[1;31m";       ///< Color rojo.
    const std::string CYAN = "\033[1;36m";      ///< Color cian.
    const std::string BLUE = "\033[1;34m";      ///< Color azul.
    const std::string MAGENTA = "\033[1;35m";   ///< Color magenta.
    const std::string BLACK = "\033[1;30m";     ///< Color negro.

    // Colores brillantes
    const std::string BRIGHT_RED = "\033[1;91m";       ///< Color rojo brillante.
    const std::string BRIGHT_GREEN = "\033[1;92m";     ///< Color verde brillante.
    const std::string BRIGHT_YELLOW = "\033[1;93m";    ///< Color amarillo brillante.
    const std::string BRIGHT_BLUE = "\033[1;94m";      ///< Color azul brillante.
    const std::string BRIGHT_MAGENTA = "\033[1;95m";   ///< Color magenta brillante.
    const std::string BRIGHT_CYAN = "\033[1;96m";      ///< Color cian brillante.

    /**
     * @brief Aplica un color y negrita a un texto dado.
     * @param text El texto al que se le aplicará el color.
     * @param color El código de color ANSI a aplicar.
     * @param bold Indica si se debe aplicar el estilo de negrita.
     * @return El texto formateado con el color y estilo especificados.
     */
    std::string applyColor(const std::string& text, const std::string& color, bool bold);

    /**
     * @brief Aplica el color blanco a un texto dado.
     * @param text El texto al que se le aplicará el color blanco.
     * @param bold Indica si se debe aplicar el estilo de negrita (opcional).
     * @return El texto formateado en color blanco.
     */
    std::string white(const std::string& text, bool bold = false);

    /**
     * @brief Aplica el color amarillo a un texto dado.
     * @param text El texto al que se le aplicará el color amarillo.
     * @param bold Indica si se debe aplicar el estilo de negrita (opcional).
     * @return El texto formateado en color amarillo.
     */
    std::string yellow(const std::string& text, bool bold = false);

    /**
     * @brief Aplica el color verde a un texto dado.
     * @param text El texto al que se le aplicará el color verde.
     * @param bold Indica si se debe aplicar el estilo de negrita (opcional).
     * @return El texto formateado en color verde.
     */
    std::string green(const std::string& text, bool bold = false);

    /**
     * @brief Aplica el color rojo a un texto dado.
     * @param text El texto al que se le aplicará el color rojo.
     * @param bold Indica si se debe aplicar el estilo de negrita (opcional).
     * @return El texto formateado en color rojo.
     */
    std::string red(const std::string& text, bool bold = false);

    /**
     * @brief Aplica el color cian a un texto dado.
     * @param text El texto al que se le aplicará el color cian.
     * @param bold Indica si se debe aplicar el estilo de negrita (opcional).
     * @return El texto formateado en color cian.
     */
    std::string cyan(const std::string& text, bool bold = false);
    /**
     * @brief Aplica el color azul a un texto dado.
     * @param text El texto al que se le aplicará el color azul.
     * @param bold Indica si se debe aplicar el estilo de negrita (opcional).
     * @return El texto formateado en color azul.
     */
    std::string blue(const std::string& text, bool bold = false);

    /**
     * @brief Aplica el color magenta a un texto dado.
     * @param text El texto al que se le aplicará el color magenta.
     * @param bold Indica si se debe aplicar el estilo de negrita (opcional).
     * @return El texto formateado en color magenta.
     */
    std::string magenta(const std::string& text, bool bold = false);

    /**
     * @brief Aplica el color negro a un texto dado.
     * @param text El texto al que se le aplicará el color negro.
     * @param bold Indica si se debe aplicar el estilo de negrita (opcional).
     * @return El texto formateado en color negro.
     */
    std::string black(const std::string& text, bool bold = false);

    /**
     * @brief Aplica el color rojo brillante a un texto dado.
     * @param text El texto al que se le aplicará el color rojo brillante.
     * @param bold Indica si se debe aplicar el estilo de negrita (opcional).
     * @return El texto formateado en color rojo brillante.
     */
    std::string brightRed(const std::string& text, bool bold = false);
    /**
     * @brief Aplica el color verde brillante a un texto dado.
     * @param text El texto al que se le aplicará el color verde brillante.
     * @param bold Indica si se debe aplicar el estilo de negrita (opcional).
     * @return El texto formateado en color verde brillante.
     */
    std::string brightGreen(const std::string& text, bool bold = false);

    /**
     * @brief Aplica el color amarillo brillante a un texto dado.
     * @param text El texto al que se le aplicará el color amarillo brillante.
     * @param bold Indica si se debe aplicar el estilo de negrita (opcional).
     * @return El texto formateado en color amarillo brillante.
     */
    std::string brightYellow(const std::string& text, bool bold = false);

    /**
     * @brief Aplica el color azul brillante a un texto dado.
     * @param text El texto al que se le aplicará el color azul brillante.
     * @param bold Indica si se debe aplicar el estilo de negrita (opcional).
     * @return El texto formateado en color azul brillante.
     */
    std::string brightBlue(const std::string& text, bool bold = false);

    /**
     * @brief Aplica el color magenta brillante a un texto dado.
     * @param text El texto al que se le aplicará el color magenta brillante.
     * @param bold Indica si se debe aplicar el estilo de negrita (opcional).
     * @return El texto formateado en color magenta brillante.
     */
    std::string brightMagenta(const std::string& text, bool bold = false);

    /**
     * @brief Aplica el color cian brillante a un texto dado.
     * @param text El texto al que se le aplicará el color cian brillante.
     * @param bold Indica si se debe aplicar el estilo de negrita (opcional).
     * @return El texto formateado en color cian brillante.
     */
    std::string brightCyan(const std::string& text, bool bold = false);
}
