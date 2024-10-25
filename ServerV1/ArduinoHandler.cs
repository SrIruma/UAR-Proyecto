using System.IO.Ports; // Importa el espacio de nombres para manejar la comunicación a través de puertos serie.
using System.Text; // Importa el espacio de nombres para trabajar con cadenas de texto y codificaciones;

/// <summary>
/// Clase que maneja la comunicación con un dispositivo Arduino a través de un puerto serie.
/// </summary>
public class ArduinoHandler
{
    private SerialPort serialPort; // Objeto que representa el puerto serie para la comunicación.
    private Logger logger; // Instancia del logger para manejar mensajes de log.
    private bool debug; // Indica si se debe activar el modo de depuración.

    /// <summary>
    /// Constructor de la clase ArduinoHandler.
    /// Inicializa el puerto serie y establece el logger y el modo de depuración.
    /// </summary>
    /// <param name="comPort">Nombre del puerto COM al que está conectado Arduino.</param>
    /// <param name="baudRate">Tasa de baudios para la comunicación.</param>
    /// <param name="logger">Instancia del logger para manejar mensajes de log.</param>
    /// <param name="debug">Indica si se activa el modo de depuración (por defecto es false).</param>
    public ArduinoHandler(string comPort, int baudRate, Logger logger, bool debug = false)
    {
        serialPort = new SerialPort(comPort, baudRate); // Inicializa el puerto serie con el puerto y la tasa de baudios especificados.
        this.logger = logger; // Asigna el logger a la clase.
        this.debug = debug; // Establece la propiedad de depuración.
    }

    /// <summary>
    /// Propiedad para habilitar o deshabilitar el modo de depuración.
    /// </summary>
    public bool Debug
    {
        get => debug; // Devuelve el estado actual del modo de depuración.
        set => debug = value; // Establece el estado del modo de depuración.
    }

    /// <summary>
    /// Abre el puerto serie para iniciar la comunicación con Arduino.
    /// Retorna un booleano.
    /// </summary>
    public bool Start()
    {
        if (serialPort.IsOpen) // Verifica si el puerto ya está abierto.
        {
            logger.LogDebug("El puerto ya está abierto."); // Registra un mensaje de depuración.
            return true; // Sale del método si el puerto ya está abierto.
        }

        try
        {
            serialPort.Open(); // Intenta abrir el puerto serie.
            logger.LogInfo($"Conexión establecida con Arduino en el puerto {serialPort.PortName}."); // Registra un mensaje de conexión exitosa.
            return true;
        }
        catch (UnauthorizedAccessException)
        {
            logger.LogError("No se puede acceder al puerto. Puede que otro proceso lo esté utilizando."); // Maneja el error de acceso no autorizado.
        }
        catch (IOException ex)
        {
            logger.LogError($"Error de entrada/salida: {ex.Message}"); // Maneja errores de entrada/salida.
        }
        catch (Exception ex)
        {
            logger.LogError($"Error al intentar abrir el puerto: {ex.Message}"); // Maneja otros errores que puedan ocurrir.
        }
        return false;
    }

    /// <summary>
    /// Lee datos del Arduino hasta que se encuentra el terminador '.'.
    /// </summary>
    /// <returns>Una cadena que contiene los datos leídos, sin el terminador '.' al final.</returns>
    public string ReadFromArduino()
    {
        if (!serialPort.IsOpen) // Verifica si el puerto está abierto.
        {
            logger.LogDebug("El puerto no está abierto."); // Registra un mensaje de depuración.
            return null; // Retorna null si el puerto no está abierto.
        }

        try
        {
            StringBuilder dataBuilder = new StringBuilder(); // Utiliza StringBuilder para construir la cadena de datos.
            char currentChar; // Variable para almacenar el carácter leído.

            while (true) // Bucle para leer hasta encontrar el terminador '.'.
            {
                currentChar = (char)serialPort.ReadChar(); // Lee un carácter del puerto serie.
                dataBuilder.Append(currentChar); // Agrega el carácter leído a la cadena de datos.
                if (currentChar == '.') // Verifica si se ha encontrado el terminador.
                {
                    break; // Sale del bucle si se encuentra el terminador.
                }
            }

            string rawData = dataBuilder.ToString(); // Convierte el contenido de StringBuilder a una cadena.
            logger.LogDebug("Datos de Arduino: " + rawData); // Registra los datos leídos en modo de depuración.
            return rawData.TrimEnd('.'); // Retorna la cadena de datos sin el terminador al final.
        }
        catch (Exception ex)
        {
            logger.LogError("Error al leer del Arduino: " + ex.Message); // Maneja cualquier excepción al leer del Arduino.
            return null; // Retorna null en caso de error.
        }
    }

    /// <summary>
    /// Cierra el puerto serie, finalizando la comunicación con Arduino.
    /// </summary>
    public void Stop()
    {
        if (serialPort.IsOpen) // Verifica si el puerto está abierto.
        {
            serialPort.Close(); // Cierra el puerto serie.
            logger.LogInfo("Conexión cerrada con Arduino."); // Registra un mensaje de cierre de conexión.
        }
    }

    /// <summary>
    /// Envía un comando al Arduino a través del puerto serie.
    /// </summary>
    /// <param name="command">El comando que se desea enviar a Arduino.</param>
    public void SendCommand(string command)
    {
        if (!serialPort.IsOpen) // Verifica si el puerto está abierto.
        {
            logger.LogDebug("El puerto no está abierto."); // Registra un mensaje de depuración si el puerto no está abierto.
            return; // Sale del método si el puerto no está abierto.
        }

        try
        {
            serialPort.WriteLine(command); // Envía el comando al puerto serie.
            logger.LogDebug("Comando enviado a Arduino: " + command); // Registra el comando enviado en modo de depuración.
        }
        catch (Exception ex)
        {
            logger.LogError("Error al enviar comando a Arduino: " + ex.Message); // Maneja cualquier excepción al enviar el comando.
        }
    }
}
