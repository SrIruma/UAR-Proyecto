public class Logger
{
    private bool debug;
    public bool Debug { get => debug; set => debug = value; }


    public Logger(bool debug = false)
    {
        this.debug = debug;
    }

    public void LogWarning(string message)
    {
        Console.ForegroundColor = ConsoleColor.White;
        Console.Write($"[");
        Console.ForegroundColor = ConsoleColor.Yellow;
        Console.Write($"WARNING");
        Console.ForegroundColor = ConsoleColor.White;
        Console.Write($"] ");
        WriteNextLog(message);
    }

    public void LogDebug(string message)
    {
        if (debug)
        {
            Console.ForegroundColor = ConsoleColor.White;
            Console.Write($"[");
            Console.ForegroundColor = ConsoleColor.Green;
            Console.Write($"DEBUG");
            Console.ForegroundColor = ConsoleColor.White;
            Console.Write($"] ");
            WriteNextLog(message);
        }
    }

    public void LogError(string message)
    {
        Console.ForegroundColor = ConsoleColor.White;
        Console.Write($"[");
        Console.ForegroundColor = ConsoleColor.Red;
        Console.Write($"ERROR");
        Console.ForegroundColor = ConsoleColor.White;
        Console.Write($"] ");
        WriteNextLog(message);
    }

    public void LogInfo(string message)
    {
        Console.ForegroundColor = ConsoleColor.White;
        Console.Write($"[");
        Console.ForegroundColor = ConsoleColor.Cyan;
        Console.Write($"INFO");
        Console.ForegroundColor = ConsoleColor.White;
        Console.Write($"] ");
        WriteNextLog(message);
    }

    private void WriteNextLog(string nextMessage)
    {
        Console.ForegroundColor = ConsoleColor.White;
        Console.Write(nextMessage + '\n');
        Console.ResetColor();
    }
}