/* This file is part of Tomat.Testament, a part of the Testament Project hosted
 * within the mono-repository located at <https://github.com/steviegt6/isaac>.
 * 
 * Tomat.Testament is released under the GNU General Public License, version 3.
 * As such, it is subject to the terms written in the `LICENSE-GPL` file located
 * within the project root.
 */

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using JetBrains.Annotations;
using Tomat.Testament.API.Logging;

namespace Tomat.Testament;

internal static class Native {
    [UnmanagedCallersOnly]
    [UsedImplicitly(ImplicitUseKindFlags.Access)]
    internal static unsafe void Initialize(short* baseDirectory) {
        AppContext.SetData("APP_CONTEXT_BASE_DIRECTORY", ReadWCharPtr(baseDirectory));
        var logger = new Logger("Tomat.Testament", LogWriter.FromMany(MakeLogWriters().ToArray()));

        logger.Info("CLR successfully initialized, hello from .NET-land!");
    }

    private static unsafe string ReadWCharPtr(short* ptr) {
        var sb = new StringBuilder();

        while (*ptr != 0) {
            sb.Append((char) *ptr);
            ptr++;
        }

        return sb.ToString();
    }

    private static IEnumerable<ILogWriter> MakeLogWriters() {
        yield return new ConsoleLogWriter();
        yield return new FileLogWriter(PrepareArchivableLogFile());
        yield return new FileLogWriter(PrepareTemporaryLogFile());
    }

    private static (string cwd, string logDir) EnsureLogDirectories() {
        var cwd = Path.GetFullPath(AppContext.BaseDirectory);
        var logDir = Path.Combine(cwd, "logs");

        Directory.CreateDirectory(logDir);

        return (cwd, logDir);
    }

    private static string PrepareArchivableLogFile() {
        var (_, logDir) = EnsureLogDirectories();
        var name = DateTime.Now.ToString("yyyy-MM-dd_HH-mm-ss") + "_testament.log";
        var logFile = Path.Combine(logDir, name);

        if (File.Exists(logFile)) {
            // TODO: Panic?
        }

        return logFile;
    }

    private static string PrepareTemporaryLogFile() {
        var (cwd, _) = EnsureLogDirectories();
        var logFile = Path.Combine(cwd, "testament.log");

        if (File.Exists(logFile)) {
            File.Delete(logFile);
        }

        return logFile;
    }
}
