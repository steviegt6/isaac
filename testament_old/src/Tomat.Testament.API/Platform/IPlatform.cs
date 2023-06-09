using System.Diagnostics;
using System.IO;

namespace Tomat.Testament.API.Platform;

/// <summary>
///     Represents a platform, which provides platform-specific abstractions for
///     general tasks.
/// </summary>
public interface IPlatform {
    /// <summary>
    ///     Starts a program with the given path and arguments.
    /// </summary>
    /// <param name="path">The path to the program to start.</param>
    /// <param name="args">The arguments for the program.</param>
    /// <param name="options">The start options.</param>
    /// <returns>The spawned process; null if process creation failed.</returns>
    Process? StartProgram(string path, string? args = null, ProcessStartOptions? options = null);

    /// <summary>
    ///     Injects a DLL into the given process.
    /// </summary>
    /// <param name="process">The process to inject into.</param>
    /// <param name="dllPath">The path to the DLL to inject.</param>
    /// <returns>
    ///     Whether the DLL was successfully injected, if knowable.
    /// </returns>
    bool InjectDll(Process process, string dllPath);
    
    /// <summary>
    ///     Gets the directory for which Testament should store its data,
    ///     located in the user's local application data directory.
    /// </summary>
    /// <returns>
    ///     The directory for which Testament should store its data.
    /// </returns>
    /// <remarks>
    ///     The actual parent directory may vary based on platform, especially
    ///     with whether it respects XDG Base Directory Specification... or if
    ///     the user is using Windows.
    /// </remarks>
    DirectoryInfo GetLocalApplicationDataDirectory();
    
    /// <summary>
    ///     Gets the directory that The Binding of Isaac: Rebirth is stored in.
    ///     <br />
    ///     This may or may not be automatically detectable.
    /// </summary>
    /// <returns>
    ///     The directory that The Binding of Isaac: Rebirth is stored in. If
    ///     one could not be resolved, null is returned.
    /// </returns>
    DirectoryInfo? GetIsaacGameDirectory();

    /// <summary>
    ///     Attempts to launch The Binding of Isaac: Rebirth, given a directory.
    /// </summary>
    /// <returns>
    ///     The process that was launched, null if failed.
    /// </returns>
    Process? LaunchIsaacGame(DirectoryInfo directory);
}
