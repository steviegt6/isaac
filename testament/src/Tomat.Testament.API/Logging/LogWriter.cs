using System;
using System.IO;
using System.Text;

namespace Tomat.Testament.API.Logging;

/// <summary>
///     Represents a log writer.
/// </summary>
public interface ILogWriter : IDisposable {
    /// <summary>
    ///     Writes (logs) a log entry.
    /// </summary>
    /// <param name="entry">The log entry to write (log).</param>
    void Write(ILogEntry entry);
}

/// <summary>
///     Abstract implementation of <see cref="ILogWriter"/> which expects
///     inheritors to implement <see cref="Write(ILogEntry)"/>.
/// </summary>
public abstract class LogWriter : ILogWriter {
    /// <summary>
    ///     A <see cref="LogWriter"/> which does nothing.
    /// </summary>
    public static readonly ILogWriter NULL = new NullLogWriter();

    public abstract void Write(ILogEntry entry);

    /// <summary>
    ///     Creates a <see cref="ManyLogWriter"/> from multiple
    ///     <see cref="ILogWriter"/>s.
    /// </summary>
    /// <param name="writers">The writers to write to.</param>
    /// <returns>An instance of <see cref="ManyLogWriter"/>.</returns>
    public static ILogWriter FromMany(ILogWriter[] writers) {
        return new ManyLogWriter(writers);
    }

    protected virtual void Dispose(bool disposing) {
        if (disposing) { }
    }

    public void Dispose() {
        Dispose(true);
        GC.SuppressFinalize(this);
    }
}

/// <summary>
///     An abstract <see cref="LogWriter"/> which handles converting a log entry
///     to a readable string.
/// </summary>
public abstract class StringLogWriter : LogWriter {
    public override void Write(ILogEntry entry) {
        var sb = new StringBuilder();
        sb.Append(entry.Level.Name.PadLeft(5));
        sb.Append(" @ ");
        sb.Append(entry.Timestamp.ToString("u"));

        if (entry.Source is not null)
            sb.Append(" by \"" + entry.Source + '\"');

        sb.Append(": ");
        sb.Append(entry.Message);

        Write(sb.ToString(), entry.Level.Color);
    }

    protected abstract void Write(string message, ConsoleColor color);
}

/// <summary>
///     A <see cref="StringLogWriter"/> which writes to the console.
/// </summary>
public sealed class ConsoleLogWriter : StringLogWriter {
    protected override void Write(string message, ConsoleColor color) {
        var oldColor = Console.ForegroundColor;
        Console.ForegroundColor = color;
        Console.WriteLine(message);
        Console.ForegroundColor = oldColor;
    }
}

/// <summary>
///     A <see cref="StringLogWriter"/> which writes to a file.
/// </summary>
public sealed class FileLogWriter : StringLogWriter {
    private readonly StreamWriter writer;

    public FileLogWriter(string path) {
        try {
            if (Directory.Exists(path))
                throw new ArgumentException("Path is a directory.", nameof(path));

            Directory.CreateDirectory(Path.GetDirectoryName(path)!);

            writer = new StreamWriter(File.OpenWrite(path));
            writer.AutoFlush = true;
        }
        catch (Exception e) {
            throw new ArgumentException("Could not open file.", nameof(path), e);
        }
    }

    protected override void Write(string message, ConsoleColor color) {
        writer.WriteLine(message);
    }

    protected override void Dispose(bool disposing) {
        if (disposing)
            writer.Dispose();

        base.Dispose(disposing);
    }
}

/// <summary>
///     A <see cref="LogWriter"/> which does nothing.
/// </summary>
public sealed class NullLogWriter : LogWriter {
    public override void Write(ILogEntry entry) { }
}

/// <summary>
///     A <see cref="LogWriter"/> which writes to multiple
///     <see cref="ILogWriter"/>s.
/// </summary>
public sealed class ManyLogWriter : LogWriter {
    private readonly ILogWriter[] writers;

    public ManyLogWriter(ILogWriter[] writers) {
        this.writers = writers;
    }

    public override void Write(ILogEntry entry) {
        foreach (var writer in writers)
            writer.Write(entry);
    }
}
