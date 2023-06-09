using System;
using System.IO;
using Tomat.Testament.API.Platform;
using Tomat.Testament.Windows;

namespace Tomat.Testament;

/// <summary>
///     Main program entry point.
/// </summary>
internal static class Program {
    /// <summary>
    ///     Main program entry point.
    /// </summary>
    /// <param name="args">Command-line launch arguments.</param>
    public static void Main(string[] args) {
        LogStartup(args);

        var platform = ResolvePlatform();
        Console.WriteLine("Using platform: " + platform.GetType().FullName);

        var saveDir = platform.GetLocalApplicationDataDirectory();
        var gameDir = platform.GetIsaacGameDirectory();
        Console.WriteLine("Using save directory: " + saveDir.FullName);
        Console.WriteLine("Using game directory: " + (gameDir?.FullName ?? "<unknown>"));

        if (gameDir is null) {
            Console.WriteLine("Could not find game directory. Please enter the path to the game directory:");

            while (gameDir is null || !gameDir.Exists) {
                var dir = Console.ReadLine();

                if (dir is null || !Directory.Exists(dir)) {
                    Console.WriteLine("Provided directory does not exist! Please enter the path to the game directory:");
                    continue;
                }

                gameDir = new DirectoryInfo(dir);
            }
        }

        Console.WriteLine("Launching game as suspended process...");
        var proc = platform.LaunchIsaacGame(gameDir);

        if (proc is null)
            throw new Exception("Failed to start game process.");

        proc.WaitForExit();
        Console.WriteLine("Isaac process exited.");
    }

    private static void LogStartup(string[] args) {
        var version = typeof(Program).Assembly.GetName().Version;
        var versionString = version?.ToString() ?? "<unknown>";

        Console.WriteLine($"Tomat.Testament v{versionString}");

        if (args.Length > 0) {
            const string msg = "Arguments provided: ";

            for (var i = 0; i < args.Length; i++) {
                Console.Write(i == 0 ? msg : new string(' ', msg.Length));
                Console.WriteLine(args[i]);
            }
        }
        else {
            Console.WriteLine("No arguments provided.");
        }
    }

    private static IPlatform ResolvePlatform() {
        if (OperatingSystem.IsWindowsVersionAtLeast(5, 1, 2600))
            return new WindowsPlatform();

        throw new PlatformNotSupportedException("Unsupported platform; no platform service available.");
    }
}
