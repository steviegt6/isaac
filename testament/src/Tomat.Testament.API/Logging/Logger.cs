using System;

namespace Tomat.Testament.API.Logging; 

/// <summary>
///     A wrapper around <see cref="ILogWriter"/>; does <b>not</b> implement
///     <see cref="ILogWriter"/>.
/// </summary>
public sealed class Logger {
    /// <summary>
    ///     The name of this logger.
    /// </summary>
    public string Name { get; }

    /// <summary>
    ///     The log writer of this logger.
    /// </summary>
    public ILogWriter LogWriter { get; }

    public Logger(string name, ILogWriter logWriter) {
        Name = name;
        LogWriter = logWriter;
    }

    /// <summary>
    ///     Logs a message with the <see cref="LogLevels.DEBUG"/> log level.
    /// </summary>
    /// <param name="message">The message to log.</param>
    public void Debug(string message) {
        Log(LogLevels.DEBUG, message);
    }

    /// <summary>
    ///     Logs a message with the <see cref="LogLevels.INFO"/> log level.
    /// </summary>
    /// <param name="message">The message to log.</param>
    public void Info(string message) {
        Log(LogLevels.INFO, message);
    }

    /// <summary>
    ///     Logs a message with the <see cref="LogLevels.WARN"/> log level.
    /// </summary>
    /// <param name="message">The message to log.</param>
    public void Warn(string message) {
        Log(LogLevels.WARN, message);
    }

    /// <summary>
    ///     Logs a message with the <see cref="LogLevels.ERROR"/> log level.
    /// </summary>
    /// <param name="message">The message to log.</param>
    public void Error(string message) {
        Log(LogLevels.ERROR, message);
    }

    private void Log(ILogLevel level, string message) {
        Write(new LogEntry(DateTime.Now, level, Name, message));
    }

    private void Write(ILogEntry entry) {
        LogWriter.Write(entry);
    }
}

/// <summary>
///     Extension methods for <see cref="Logger"/>.
/// </summary>
public static class LoggerExtensions {
    /// <summary>
    ///     Creates a child logger with the given name, appended to the name of
    ///     this logger, separated with <c>"::"</c>.
    /// </summary>
    /// <param name="logger">The logger to make a child from.</param>
    /// <param name="name">The name of this child.</param>
    /// <returns>A child <see cref="Logger"/> instance.</returns>
    public static Logger MakeChildWithName(this Logger logger, string name) {
        return new Logger(logger.Name + "::" + name, logger.LogWriter);
    }

    /// <summary>
    ///     Creates a child logger with the given type's name, appended to the
    ///     name of this logger, separated with <c>"::"</c>.
    /// </summary>
    /// <param name="logger">The logger to make a child from.</param>
    /// <param name="type">
    ///     The type from which the child's name is to be derived from.
    /// </param>
    /// <returns>A child <see cref="Logger"/> instance.</returns>
    public static Logger MakeChildFromType(this Logger logger, Type type) {
        var name = type.FullName ?? type.Name;

        // In the event FullName is null and we're just given Name, we can
        // simply not bother with name shortening.
        if (!name.Contains('.'))
            return logger.MakeChildWithName(name);

        var assemblyName = type.Assembly.GetName().Name;

        // If we don't know the assembly's name, we can't leave it in while
        // shortening namespaces. This shouldn't ever really be null, so a good
        // indicator that it somehow *is* is to simply leave the entire name
        // untouched. Makes it easier for me, anyway.
        if (assemblyName is null)
            return logger.MakeChildWithName(name);

        var nameParts = name[(assemblyName.Length + 1)..].Split('.');

        // Shorten each namespace part to just four letters at most, but leave
        // the type name unchanged.
        for (var i = 0; i < nameParts.Length - 1; i++) {
            if (nameParts[i].Length > 4)
                nameParts[i] = nameParts[i][..4];
        }

        return logger.MakeChildWithName(assemblyName + '.' + string.Join('.', nameParts));
    }
}
