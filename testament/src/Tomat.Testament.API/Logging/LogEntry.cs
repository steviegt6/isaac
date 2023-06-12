using System;

namespace Tomat.Testament.API.Logging;

/// <summary>
///     Represents a log entry.
/// </summary>
public interface ILogEntry {
    /// <summary>
    ///     The timestamp of the log entry; when this entry was created.
    /// </summary>
    DateTime Timestamp { get; }

    /// <summary>
    ///     The log level of the log entry.
    /// </summary>
    ILogLevel Level { get; }

    /// <summary>
    ///     The source of the log entry.
    /// </summary>
    string? Source { get; }

    /// <summary>
    ///     The message of the log entry.
    /// </summary>
    string Message { get; }
}

/// <summary>
///     A simple <see cref="ILogEntry"/> implementation which has its properties
///     populated by a constructor.
/// </summary>
/// <param name="Timestamp">
///     The timestamp of the log entry; when this entry was created.
/// </param>
/// <param name="Level">The log level of the log entry.</param>
/// <param name="Source">The source of the log entry.</param>
/// <param name="Message">The message of the log entry.</param>
public readonly record struct LogEntry(DateTime Timestamp, ILogLevel Level, string? Source, string Message) : ILogEntry;
