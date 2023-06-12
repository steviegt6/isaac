using System;

namespace Tomat.Testament.API.Logging;

/// <summary>
///     Represents a log level.
/// </summary>
public interface ILogLevel {
    /// <summary>
    ///     The name of the log level.
    /// </summary>
    string Name { get; }

    /// <summary>
    ///     The weighted priority of the log level.
    /// </summary>
    float Priority { get; }

    /// <summary>
    ///     The color of the log level.
    /// </summary>
    ConsoleColor Color { get; }
}

/// <summary>
///     A simple <see cref="ILogLevel"/> implementation which has its properties
///     populated by a constructor.
/// </summary>
/// <param name="Name">The name of the log level.</param>
/// <param name="Priority">The weighted priority of the log level.</param>
/// <param name="Color">The color of the log level.</param>
public readonly record struct LogLevel(string Name, float Priority, ConsoleColor Color) : ILogLevel;
