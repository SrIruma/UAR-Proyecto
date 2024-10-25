class Program
{
    private static readonly string Host = "0.0.0.0";
    private static int Port = 25565;
    private static string ComPort = "COM3";
    private static int BaudRate = 9600;
    private static int MaxConnections = 5;
    private static bool DebugMode = false;

    private static TcpServer tcpServer;
    private static ArduinoHandler handler;
    private static Logger logger;

    private static readonly int[] ReservedTcpPorts = {
    0, 7, 9, 13, 17, 19, 20, 21, 22, 23, 25,
    53, 67, 68, 69, 80, 110, 135, 137, 138, 139,
    143, 161, 162, 389, 443, 445, 464, 514, 515,
    520, 587, 631, 993, 995 };

    static void Main(string[] args)
    {
        logger = new Logger();
        Initializer();
        CommandLineInterface();
    }

    private static void CommandLineInterface()
    {
        ShowProjectInfo();

        while (true)
        {
            DisplayCommander();
            string command = Console.ReadLine();

            if (command.Equals("--kill", StringComparison.OrdinalIgnoreCase) || command.Equals("-k", StringComparison.OrdinalIgnoreCase))
            {
                StopServer();
                Thread.Sleep(1500);
                Environment.Exit(0);
            }

            ProcessCommand(command);
        }
    }

    private static readonly Dictionary<string, Action<string[]>> commands = new Dictionary<string, Action<string[]>>
    {
    { "-h", args => PrintCommands() },
    { "--help", args => PrintCommands() },
    { "-i", args => ShowProjectInfo() },
    { "--info", args => ShowProjectInfo() },
    { "-cl", args => { Console.Clear(); Initializer(); } },
    { "--clear", args => { Console.Clear(); Initializer(); } },
    { "-p", UpdatePort },
    { "--port", UpdatePort },
    { "-c", UpdateComPort },
    { "--com-port", UpdateComPort },
    { "-b", UpdateBaudRate },
    { "--baudrate", UpdateBaudRate },
    { "-m", UpdateMaxConnections },
    { "--max-connections", UpdateMaxConnections },
    { "-d", PrintDefaults },
    { "--display-defaults", PrintDefaults },
    { "-r", RunServer },
    { "--run", RunServer },
    { "-k", args => StopServer() },
    { "--kill", args => StopServer() }
    };

    private static void ProcessCommand(string command)
    {
        var args = command.Split(' ', StringSplitOptions.RemoveEmptyEntries);
        if (args.Length == 0 || !commands.ContainsKey(args[0].ToLower()))
        {
            Console.WriteLine("Comando desconocido. Usa -h para ver las opciones.");
            return;
        }
        commands[args[0].ToLower()](args);
    }

    private static void UpdatePort(string[] args)
    {
        if (args.Length > 1 && int.TryParse(args[1], out int port))
        {
            if (port >= 1 && port <= 65535 && !ReservedTcpPorts.Contains(port))
            {
                Port = port;
                logger.LogInfo($"Puerto configurado: {Port}");
            }
            else
            {
                logger.LogError("Debes especificar un número de puerto válido (1-65535) y no reservado.");
            }
        }
        else
        {
            logger.LogError("Debes especificar un número de puerto válido.");
        }
    }

    private static void UpdateComPort(string[] args)
    {
        if (args.Length > 1)
        {
            string comPort = args[1].ToUpper();
            if (comPort.StartsWith("COM") && int.TryParse(comPort[3..], out int comNumber) && comNumber >= 1 && comNumber <= 255)
            {
                ComPort = comPort;
                logger.LogInfo($"Puerto COM configurado: {ComPort}");
            }
            else
            {
                logger.LogError("Debes especificar un puerto COM válido (ej. COM1, COM2, ... COM255).");
            }
        }
        else
        {
            logger.LogError("Debes especificar un puerto COM.");
        }
    }

    private static void UpdateBaudRate(string[] args)
    {
        if (args.Length > 1 && int.TryParse(args[1], out int baudrate))
        {
            BaudRate = baudrate;
            logger.LogInfo($"Tasa de baudios configurada: {BaudRate}");
        }
        else
        {
            logger.LogError("Debes especificar una tasa de baudios válida.");
        }
    }

    private static void UpdateMaxConnections(string[] args)
    {
        if (args.Length > 1 && int.TryParse(args[1], out int maxConnections) && maxConnections >= 1 && maxConnections <= 10)
        {
            MaxConnections = maxConnections;
            logger.LogInfo($"Número máximo de conexiones configurado: {MaxConnections}");
        }
        else
        {
            logger.LogError("Debes especificar un número de conexiones válido (1-10).");
        }
    }

    private static void RunServer(string[] args)
    {
        if (args.Length > 1 && args[1] == "--tcp")
        {
            DebugMode = args.Length > 2 && args[2] == "--debug";
            StartTcpServer();
        }
        else
        {
            logger.LogError("Uso Incorrecto. Use -h para ver las opciones disponibles.");
        }
    }

    private static void StartTcpServer()
    {
        PrintDefaults([]);

        logger.LogInfo("¿Desea iniciar el servidor con esta configuración? (S/N): ");
        ConsoleKey keyPressed;
        do
        {
            keyPressed = Console.ReadKey(true).Key;
        }
        while (keyPressed != ConsoleKey.S && keyPressed != ConsoleKey.N && keyPressed != ConsoleKey.Y);

        if (keyPressed == ConsoleKey.S || keyPressed == ConsoleKey.Y)
        {
            if (tcpServer == null)
            {
                InitServer();
            }
            else
            {
                tcpServer.Stop();
                tcpServer = null;
                InitServer();
            }
        }
        else if (keyPressed == ConsoleKey.N)
        {
            Console.Clear();
            PrintLogo();
            ShowProjectInfo();
        }
    }
    private static void InitServer()
    {
        logger = null; logger = new Logger(DebugMode);
        handler = new ArduinoHandler(ComPort, BaudRate, logger, DebugMode);
        tcpServer = new TcpServer(Host, Port, handler, MaxConnections, logger, DebugMode);
        if (tcpServer.Start())
        {
            DisplaySpecialKeys();
            MonitorServer();
        }
    }
    private static void DisplaySpecialKeys()
    {
        string[] nLangArt =
        [
        "╔══════════════════════════════════════════════════════════════════════════════════════════════════════╗",
        "║                                  TECLAS ESPECIALES DISPONIBLES                                       ║",
        "╠══════════════════════════════════════════════════════════════════════════════════════════════════════╣",
        "║      F8  : Limpiar la consola                                                                        ║",
        "║      F9  : Detener el servidor y cerrar todas las conexiones activas                                 ║",
        "║      F10 : Activar/Desactivar el modo de depuración                                                  ║",
        "╚══════════════════════════════════════════════════════════════════════════════════════════════════════╝",
        ];
        foreach (string line in nLangArt)
        {
            Console.ResetColor();
            foreach (char c in line)
            {
                Console.ForegroundColor = c switch
                {
                    '░' => Console.ForegroundColor = ConsoleColor.Red,
                    '█' => Console.ForegroundColor = ConsoleColor.Gray,
                    '═' or '║' or '╝' or '╚' or '╔' or '╗' or '╦' or '╠' or '╣' => ConsoleColor.Blue,
                    _ => ConsoleColor.White,
                };
                Console.Write(c);
            }
            Console.ResetColor();
            Console.WriteLine();
        }
    }

    private static void MonitorServer()
    {
        while (tcpServer?.IsRunning == true)
        {
            CheckKeyPresses();
            Thread.Sleep(100);
        }
        tcpServer = null;
    }

    private static void CheckKeyPresses()
    {
        if (Console.KeyAvailable)
        {
            var key = Console.ReadKey(true).Key;
            if (key == ConsoleKey.F8)
            {
                Console.Clear();
                Initializer();
                DisplaySpecialKeys();
            }
            else if (key == ConsoleKey.F9)
                StopServer();
            else if (key == ConsoleKey.F10)
                ToggleDebugMode();
        }
    }

    private static void ToggleDebugMode()
    {
        DebugMode = !DebugMode;
        tcpServer.Debug = DebugMode;
        handler.Debug = DebugMode;
        logger.Debug = DebugMode;
        logger.LogWarning($"Modo Depuración: {(DebugMode ? "Activado" : "Desactivado")}");
    }

    public static void StopServer()
    {
        if (tcpServer == null || !tcpServer.IsRunning)
        {
            logger.LogInfo("No hay servidores en ejecución para detener.");
            return;
        }

        logger.LogInfo("Deteniendo servidor TCP, espere un momento...");
        tcpServer.Stop();
        Thread.Sleep(1500);
        logger.LogInfo("Finalizando el cierre del servidor...");
        ShowLoadingAnimation();
        Thread.Sleep(1000);
        Console.Clear();
        PrintLogo();
        ShowProjectInfo();
    }

    private static void ShowLoadingAnimation()
    {
        char[] loadingChars = { '\\', '/', '-', '|' };
        int totalLoaders = 53;
        int delay = 200;
        Random random = new Random();

        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < totalLoaders; j++)
            {
                Console.ForegroundColor = (ConsoleColor)random.Next(1, 16);
                Console.SetCursorPosition(j * 2, Console.CursorTop);
                Console.Write(loadingChars[(i + j) % loadingChars.Length]);
            }
            Thread.Sleep(delay);
            Console.SetCursorPosition(0, Console.CursorTop);
        }

        Console.ResetColor();
    }

    #region PRINTERS

    static void PrintDefaults(string[] objs)
    {
        string[] defsInfo =
        [
            "╔══════════════════════════════════════════════════════════════════════════════════════════════════════╗",
            "║                                    CONFIGURACIÓN POR DEFECTO                                         ║",
            "╠══════════════════════════════════════════════════════════════════════════════════════════════════════╣",
            $"║          HOST                    : {Host}                                                           ║",
            $"║          PUERTO TCP              : {Port}                                                             ║",
            $"║          PUERTO ARDUINO          : {ComPort}                                                              ║",
            $"║          BAUD RATE               : {BaudRate}                                                              ║",
            $"║          MÁXIMO DE CONEXIONES    : {MaxConnections}                                                                 ║",
            $"║          ESTADO DEL SERVIDOR     : {(tcpServer?.IsRunning == true ? "En Ejecución" : "Inactivo")}                                                          ║",
            $"║          MODO DEPURACIÓN         : {(DebugMode ? "Activado" : "Apagado ")}                                                          ║",
            "╚══════════════════════════════════════════════════════════════════════════════════════════════════════╝",
        ];

        foreach (string line in defsInfo)
        {
            Console.ResetColor();
            foreach (char c in line)
            {
                Console.ForegroundColor = c switch
                {
                    '═' or '║' or '╝' or '╚' or '╔' or '╗' or '╠' or '╣' => ConsoleColor.Blue,
                    _ => ConsoleColor.White,
                };
                Console.Write(c);
            }
            Console.ResetColor();
            Console.WriteLine();
        }
    }

    static void DisplayCommander()
    {
        Console.ForegroundColor = ConsoleColor.Red;
        Console.Write("@");
        Console.ForegroundColor = ConsoleColor.Yellow;
        Console.Write("UAR-UCENM");
        Console.ForegroundColor = ConsoleColor.Blue;
        Console.Write(" ► ");
        Console.ResetColor();
    }

    static void ShowProjectInfo()
    {
        string[] projectInfo =
        {
            "╔══════════════════════════════════════════════════════════════════════════════════════════════════════╗",
            "║                                SERVIDOR DE RADAR ULTRASÓNICO PROYECTO                                ║",
            "╠══════════════════════════════════════════════════════════════════════════════════════════════════════╣",
            "║ Nombre del Proyecto   : Servidor de Radar Ultrasónico UAR-S®                                         ║",
            "║ Versión               : v1.0.3 build_259                                                             ║",
            "║ Fecha de Lanzamiento  : Octubre 2024                                                                 ║",
            "║ Desarrollado Por      : Estudiantes de Ingeniería en Sistemas | UCENM                                ║",
            "╠══════════════════════════════════════════════════════════════════════════════════════════════════════╣",
            "║ Descripción           : Este proyecto utiliza un sensor de radar ultrasónico para detectar objetos   ║",
            "║                         en su camino y enviar datos a una aplicación móvil. El componente servidor   ║",
            "║                         componente servidor está hecho en C# y gestiona las conexiones entre Arduino ║",
            "║                         y dispositivos móviles.                                                      ║",
            "╠══════════════════════════════════════════════════════════════════════════════════════════════════════╣",
            "║ Características       : - Comunicación Bluetooth/TCP-IP                                              ║",
            "║                         - Detección de objetos en tiempo real                                        ║",
            "║                         - Configuración ajustable del servidor                                       ║",
            "╠══════════════════════════════════════════════════════════════════════════════════════════════════════╣",
            "║ ¿Necesitas ayuda?     : - Usa el comando -h o --help para abrir el menu de ayuda.                    ║",
            "╚══════════════════════════════════════════════════════════════════════════════════════════════════════╝",
        };
        foreach (string line in projectInfo)
        {
            Console.ResetColor();
            foreach (char c in line)
            {
                Console.ForegroundColor = c switch
                {
                    '®' => ConsoleColor.Yellow,
                    '═' or '║' or '╝' or '╚' or '╔' or '╗' or '╦' or '╠' or '╣' => ConsoleColor.Blue,
                    _ => ConsoleColor.White
                };
                Console.Write(c);
            }
            Console.WriteLine();
        }
    }

    static void PrintCommands()
    {
        string[] commandsInfo =
        [
        "╔══════════════════════════════════════════════════════════════════════════════════════════════════════╗",
        "║                                         COMANDOS DISPONIBLES                                         ║",
        "╠══════════════════════════════════════════════════════════════════════════════════════════════════════╣",
        "║ -h,  --help                   : Muestra este menu de ayuda                                           ║",
        "║ -i,  --info                   : Muestra más información de este programa                             ║",
        "║ -d,  --display-defaults       : Muestra la configuración por defecto del servidor                    ║",
        "║ -p,  --port [puerto]          : Establece el puerto del servidor (default: 25565)                    ║",
        "║ -c,  --com-port [COM]         : Establece el puerto COM de Arduino (default: COM3)                   ║",
        "║ -b,  --baudrate [baud]        : Establece la tasa de baudios (default: 9600)                         ║",
        "║ -m,  --max-connections [1-10] : Establece el número máximo de conexiones (default: 5)                ║",
        "║ -r,  --run --tcp [--debug]    : Enciende el servidor TCP y abre las conexiones con Arduino (ESTABLE) ║",
        "║ -k,  --kill                   : Cierra el servidor y por ende el programa                            ║",
        "║ -cl, --clear                  : Limpia la consola                                                    ║",
        "╚══════════════════════════════════════════════════════════════════════════════════════════════════════╝",
        ];

        foreach (string line in commandsInfo)
        {
            Console.ResetColor();
            foreach (char c in line)
            {
                Console.ForegroundColor = c switch
                {
                    '═' or '║' or '╝' or '╚' or '╔' or '╗' or '╠' or '╣' => ConsoleColor.Blue,
                    _ => ConsoleColor.White,
                };
                Console.Write(c);
            }
            Console.ResetColor();
            Console.WriteLine();
        }
    }

    private static void Initializer()
    {
        Console.Title = "ULTRASONIC RADAR SERVER PROYECT - UCENM -";
        SetConsoleSize(106);
        PrintLogo();
    }

    private static void SetConsoleSize(int newWidth)
    {
        if (Console.WindowWidth > newWidth)
        {
            try
            {
                Console.SetWindowSize(newWidth, Console.WindowHeight);
                Console.BufferWidth = newWidth;
            }
            catch (IOException)
            {
                Console.WriteLine("No se pudo cambiar el tamaño de la consola.");
            }
        }
    }
    private static void PrintLogo()
    {
        string[] nLangArt =
        [
        "════════════════════════════════════════════════════════════════════════════════════════════════════════",
        "░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░",
        "░░░█████╗░░█████╗░░░████╗░░███░░░░███░█████╗░░█████╗████████░████╗░░░░░██╗░░░██╗░░█████╗░░░░░█████╗░░░░░",
        "░░░██╔══██╗██╔══██╗██╔══██╗░╚██░░██╝░░██╔══╝██╔════╝╚═╗██╔═╝██╔══██╗░░░██║░░░██║░░██╔══██╗░░░██╔══██░░░░",
        "░░░█████╔═╝█████╦═╝██║░░██║░░░║██║░░░░█████╗██║░░░░░░░║██║░░██║░░██║░░░██║░░░██║░░███████║░░░█████║░░░░░",
        "░░░██╔══╝░░██╔══██╗██║░░██║░░░║██║░░░░██╔══╝██║░░░░░░░║██║░░██║░░██║░░░██║░░░██║░░██╔══██║░░░██╔══██╗░░░",
        "░░░██║░░░░░██║░░██║╚█████╔╝░░░║██║░░░░██████╗░█████╗░░║██║░░╚█████╔╝░░░╚█████╔═╝░░██║░░██║░░░██║░░██║░░░",
        "░░░╚═╝░░░░░╚═╝░░╚═╝░╚════╝░░░░╚══╝░░░░╚═════╝░╚════╝░░╚══╝░░░╚════╝░░░░░░╚═══╝░░█░╚═╝░░╚═╝░█░╚═╝░░╚═╝░░░",
        "░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░",
        "════════════════════════════════════════════════════════════════════════════════════════════════════════",
        ];
        foreach (string line in nLangArt)
        {
            Console.ResetColor();
            foreach (char c in line)
            {
                Console.ForegroundColor = c switch
                {
                    '░' => Console.ForegroundColor = ConsoleColor.Red,
                    '█' => Console.ForegroundColor = ConsoleColor.Gray,
                    '═' or '║' or '╝' or '╚' or '╔' or '╗' or '╦' or '╠' or '╣' => ConsoleColor.Blue,
                    _ => ConsoleColor.White,
                };
                Console.Write(c);
            }
            Console.ResetColor();
            Console.WriteLine();
        }
    }

    #endregion
}