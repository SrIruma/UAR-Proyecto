using System.Net; // Importa el espacio de nombres para manejar direcciones IP y puertos.
using System.Net.Sockets; // Importa el espacio de nombres para manejar conexiones TCP/IP.
using System.Text; // Importa el espacio de nombres para trabajar con cadenas de texto y codificaciones.

/// <summary>
/// Clase que implementa un servidor TCP para manejar conexiones de clientes y comunicarse con un dispositivo Arduino.
/// </summary>
public class TcpServer
{
    private TcpListener server; // Objeto que escucha conexiones TCP entrantes.
    private ArduinoHandler arduinoHandler; // Instancia de ArduinoHandler para comunicarse con el dispositivo Arduino.
    private bool isRunning; // Indica si el servidor está en funcionamiento.
    private List<TcpClient> clients; // Lista de clientes conectados.
    private int maxConnections; // Número máximo de conexiones de clientes permitidas.
    private Logger? logger; // Instancia del logger para manejar mensajes de log.
    private bool debug; // Indica si se debe activar el modo de depuración.

    /// <summary>
    /// Propiedad que indica si el servidor está en funcionamiento.
    /// </summary>
    public bool IsRunning => isRunning;

    /// <summary>
    /// Propiedad para habilitar o deshabilitar el modo de depuración.
    /// </summary>
    public bool Debug
    {
        get => debug; // Devuelve el estado actual del modo de depuración.
        set => debug = value; // Establece el estado del modo de depuración.
    }

    /// <summary>
    /// Constructor de la clase TcpServer.
    /// Inicializa el servidor TCP, el ArduinoHandler y establece el número máximo de conexiones.
    /// </summary>
    /// <param name="host">Dirección IP del host donde se ejecuta el servidor.</param>
    /// <param name="port">Puerto donde escucha el servidor.</param>
    /// <param name="arduinoHandler">Instancia de ArduinoHandler para la comunicación con el Arduino.</param>
    /// <param name="maxConnections">Número máximo de conexiones de clientes permitidas.</param>
    /// <param name="logger">Instancia del logger para manejar mensajes de log.</param>
    /// <param name="debug">Indica si se activa el modo de depuración (por defecto es false).</param>
    public TcpServer(string host, int port, ArduinoHandler arduinoHandler, int maxConnections = 5, Logger logger = null, bool debug = false)
    {
        server = new TcpListener(IPAddress.Parse(host), port); // Inicializa el TcpListener con la dirección IP y el puerto especificados.
        this.arduinoHandler = arduinoHandler; // Asigna el ArduinoHandler a la clase.
        clients = new List<TcpClient>(); // Inicializa la lista de clientes conectados.
        this.maxConnections = maxConnections; // Establece el número máximo de conexiones permitidas.
        this.logger = logger ?? new Logger(); // Asigna el logger a la clase o crea uno por defecto.
        this.debug = debug; // Establece la propiedad de depuración.
    }

    /// <summary>
    /// Inicia el servidor TCP y comienza a aceptar conexiones de clientes.
    /// </summary>
    public bool Start()
    {
        if (isRunning) // Verifica si el servidor ya está en ejecución.
        {
            logger.LogError("El servidor ya está en ejecución."); // Registra un mensaje de conexión ya existente.
            return true; // Sale del método si el servidor ya está en ejecución.
        }

        server.Start(); // Inicia el servidor TCP.
        isRunning = true; // Cambia el estado del servidor a en ejecución.
        if (!arduinoHandler.Start())
        {
            logger.LogError("No se pudo establecer conexión con el Arduino. El servidor no se iniciará.");
            isRunning = false;
            return false; // Si falla la conexión con Arduino, no continuamos.
        }
        arduinoHandler.Start(); // Inicia la comunicación con Arduino.

        string localIp = GetLocalIPAddress(); // Obtiene la dirección IP local.
        logger.LogInfo($"Servidor TCP en ejecución [{localIp}:{server.Server.LocalEndPoint.ToString().Split(':')[1]}], esperando conexiones..."); // Registra que el servidor está esperando conexiones.

        new Thread(AcceptClients).Start(); // Inicia un hilo para aceptar clientes.
        new Thread(ReadAndBroadcastArduinoData).Start(); // Inicia un hilo para leer y transmitir datos de Arduino.
        return true;
    }

    /// <summary>
    /// Acepta conexiones de clientes de manera asíncrona.
    /// </summary>
    private async void AcceptClients()
    {
        try
        {
            while (isRunning) // Mientras el servidor esté en ejecución.
            {
                if (clients.Count >= maxConnections) // Verifica si se ha alcanzado el número máximo de conexiones.
                {
                    logger.LogWarning("Número máximo de conexiones alcanzado."); // Registra que se ha alcanzado el límite.
                    await Task.Delay(1000); // Espera un segundo antes de seguir verificando.
                    continue; // Continúa verificando si hay nuevas conexiones.
                }

                TcpClient client = await server.AcceptTcpClientAsync(); // Acepta una conexión de cliente de manera asíncrona.
                if (!isRunning) break; // Si el servidor ya no está en ejecución, sale del bucle.
                clients.Add(client); // Agrega el cliente a la lista de clientes conectados.
                logger.LogInfo("Cliente conectado."); // Registra que un cliente se ha conectado.
                new Thread(() => HandleClient(client)).Start(); // Inicia un hilo para manejar la comunicación con el cliente.
            }
        }
        catch (SocketException ex)
        {
            logger.LogError("Error en AcceptClients: " + ex.Message); // Registra errores de socket.
        }
        catch (Exception ex)
        {
            logger.LogError("Error inesperado en AcceptClients: " + ex.Message); // Registra cualquier otro error inesperado.
        }
    }

    /// <summary>
    /// Maneja la comunicación con un cliente específico.
    /// </summary>
    /// <param name="client">El cliente TCP que se va a manejar.</param>
    private void HandleClient(TcpClient client)
    {
        using (NetworkStream stream = client.GetStream()) // Obtiene el flujo de red del cliente.
        {
            byte[] buffer = new byte[1024]; // Buffer para almacenar datos recibidos.
            int bytesRead; // Variable para contar los bytes leídos.

            try
            {
                while ((bytesRead = stream.Read(buffer, 0, buffer.Length)) != 0) // Lee datos del cliente.
                {
                    string data = Encoding.ASCII.GetString(buffer, 0, bytesRead); // Convierte los bytes leídos a una cadena.
                    logger.LogDebug("[CLIENT] " + data); // Registra el mensaje recibido en modo de depuración.

                    byte[] response = Encoding.ASCII.GetBytes("Datos recibidos: "); // Crea una respuesta.
                    stream.Write(response, 0, response.Length); // Envía la respuesta al cliente.
                    logger.LogDebug("[CLIENT] Datos recibidos: "); // Registra la respuesta enviada en modo de depuración.
                }
            }
            catch (Exception ex)
            {
                logger.LogError("Error en el cliente: " + ex.Message); // Registra errores durante la comunicación con el cliente.
            }
            finally
            {
                clients.Remove(client); // Remueve al cliente de la lista de clientes conectados.
                logger.LogInfo($"Cliente desconectado: {client.Client.LocalEndPoint}"); // Registra que un cliente se ha desconectado.
            }
        }
    }

    /// <summary>
    /// Lee datos del Arduino y los transmite a todos los clientes conectados.
    /// </summary>
    private void ReadAndBroadcastArduinoData()
    {
        while (isRunning) // Mientras el servidor esté en ejecución.
        {
            string arduinoData = arduinoHandler.ReadFromArduino(); // Lee datos de Arduino.
            if (arduinoData != null) // Verifica si se han recibido datos.
            {
                arduinoData = arduinoData.Replace(".", ""); // Elimina el terminador de los datos.
                BroadcastToClients(arduinoData + '\n'); // Transmite los datos a todos los clientes conectados.
            }

            Thread.Sleep(1); // Espera un milisegundo antes de la siguiente lectura.
        }
    }

    /// <summary>
    /// Transmite un mensaje a todos los clientes conectados.
    /// </summary>
    /// <param name="message">El mensaje que se va a transmitir.</param>
    private void BroadcastToClients(string message)
    {
        byte[] data = Encoding.ASCII.GetBytes(message); // Convierte el mensaje a bytes.

        foreach (var client in clients) // Itera sobre cada cliente conectado.
        {
            if (client.Connected) // Verifica si el cliente sigue conectado.
            {
                try
                {
                    NetworkStream stream = client.GetStream(); // Obtiene el flujo de red del cliente.
                    stream.Write(data, 0, data.Length); // Envía el mensaje al cliente.
                    logger.LogDebug($"Mensaje enviado a un cliente: {message}"); // Registra el mensaje enviado en modo de depuración.
                }
                catch (Exception ex)
                {
                    logger.LogError($"Error al enviar mensaje a un cliente: {ex.Message}"); // Registra errores al enviar mensajes.
                }
            }
        }
    }

    /// <summary>
    /// Detiene el servidor TCP y cierra todas las conexiones de clientes.
    /// </summary>
    public void Stop()
    {
        isRunning = false; // Cambia el estado del servidor a no en ejecución.
        foreach (var client in clients) // Itera sobre cada cliente conectado.
        {
            client.Close(); // Cierra la conexión con el cliente.
        }
        arduinoHandler.Stop();
        server.Stop(); // Detiene el servidor TCP.
        logger.LogInfo("Servidor TCP detenido."); // Registra que el servidor ha sido detenido.
    }

    /// <summary>
    /// Obtiene la dirección IP local de la máquina.
    /// </summary>
    /// <returns>Dirección IP local en formato de cadena.</returns>
    private string GetLocalIPAddress()
    {
        var host = Dns.GetHostEntry(Dns.GetHostName()); // Obtiene la entrada DNS del host local.
        foreach (var ip in host.AddressList) // Itera sobre la lista de direcciones IP.
        {
            if (ip.AddressFamily == AddressFamily.InterNetwork) // Verifica si la dirección es de tipo IPv4.
            {
                return ip.ToString(); // Retorna la dirección IP en formato de cadena.
            }
        }
        throw new Exception("No se pudo encontrar una dirección IP válida."); // Lanza una excepción si no se encuentra una dirección IP.
    }
}