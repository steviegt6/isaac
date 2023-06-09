using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.Versioning;
using System.Text;
using Microsoft.Win32;
using Tomat.Testament.API.Platform;
using Vanara.PInvoke;

namespace Tomat.Testament.Windows;

[SupportedOSPlatform("windows5.1.2600")]
public sealed class WindowsPlatform : IPlatform {
    private const int isaac_steam_app_id = 250900;
    private const int isaac_gog_app_id = 1205572215;

    [SupportedOSPlatform("windows5.1.2600")]
    public Process? StartProgram(string path, string? args = null, ProcessStartOptions? options = null) {
        options ??= new ProcessStartOptions();
        args ??= "";

        var startupInformation = new Kernel32.STARTUPINFO();
        var processAttributes = new SECURITY_ATTRIBUTES();
        var threadAttributes = new SECURITY_ATTRIBUTES();

        var success = Kernel32.CreateProcess(
            path,
            new StringBuilder(args),
            processAttributes,
            threadAttributes,
            false,
            options.StartFrozen ? Kernel32.CREATE_PROCESS.CREATE_SUSPENDED : 0,
            null,
            Path.GetDirectoryName(path),
            in startupInformation,
            out var processInformation
        );

        return !success ? null : Process.GetProcessById((int)processInformation.dwProcessId);
    }

    [SupportedOSPlatform("windows5.1.2600")]
    public bool InjectDll(Process process, string dllPath) {
        var processHandle = new HPROCESS(process.Handle);

        var kernelHandle = Kernel32.GetModuleHandle("kernel32.dll");
        var loadLibPtr = Kernel32.GetProcAddress(kernelHandle, "LoadLibraryW");

        var strSize = (dllPath.Length + 1 /* null terminator */) * sizeof(char);
        var dllPathStringPtr = Kernel32.VirtualAllocEx(
            processHandle,
            nint.Zero,
            strSize,
            Kernel32.MEM_ALLOCATION_TYPE.MEM_COMMIT
          | Kernel32.MEM_ALLOCATION_TYPE.MEM_RESERVE,
            Kernel32.MEM_PROTECTION.PAGE_READWRITE
        );

        var writtenSuccessfully = Kernel32.WriteProcessMemory(processHandle, dllPathStringPtr, Encoding.UTF8.GetBytes(dllPath).Append((byte)0).ToArray(), strSize, out var bytesWritten);
        if (!writtenSuccessfully)
            return false;

        if (bytesWritten != strSize) {
            Kernel32.VirtualFreeEx(processHandle, dllPathStringPtr, 0, Kernel32.MEM_ALLOCATION_TYPE.MEM_RELEASE);
            return false;
        }

        Kernel32.CreateRemoteThread(processHandle, null, 0, loadLibPtr, dllPathStringPtr, 0, out _);
        return true;
    }

    public DirectoryInfo GetLocalApplicationDataDirectory() {
        // On windows we can just use LocalAppData.
        var appDataDir = Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData);
        var testamentDir = Path.Combine(appDataDir, "testament");
        Directory.CreateDirectory(testamentDir);

        return new DirectoryInfo(testamentDir);
    }

    public DirectoryInfo? GetIsaacGameDirectory() {
        foreach (var path in GetPossibleGameDirectories()) {
            if (path is not null && Directory.Exists(path))
                return new DirectoryInfo(path);
        }

        return null;
    }

    private IEnumerable<string?> GetPossibleGameDirectories() {
        string? ReturnIfExists(string directory) {
            return Directory.Exists(directory) ? directory : null;
        }

        yield return Registry.GetValue($@"HKEY_LOCAL_MACHINE\SOFTWARE\GOG.com\Games\{isaac_gog_app_id}", "PATH", null) as string;
        yield return Registry.GetValue($@"HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Steam App {isaac_steam_app_id}", "InstallLocation", null) as string;

        if (Registry.GetValue(@"HKEY_CURRENT_USER\SOFTWARE\Valve\Steam", "SteamPath", null) is string steamLibPath)
            yield return Path.Combine(steamLibPath, "steamapps", "common", "The Binding of Isaac Rebirth");

        /* TODO: Need the GOG name for TBoI:R (or Repentance specifically?):
         *  C:\Program Files\GalaxyClient\Games\NameHere
         *  C:\Program Files\GOG Galaxy\Games\NameHere
         *  C:\Program Files (x86)\GalaxyClient\Games\NameHere
         *  C:\Program Files (x86)\GOG Galaxy\Games\NameHere
         *  C:\program Files (x86)\GOG Games\Games\NameHere
         */

        if (ReturnIfExists(@"C:\Program Files\Steam\steamapps\common\The Binding of Isaac Rebirth") is { } path64)
            yield return path64;

        if (ReturnIfExists(@"C:\Program Files (x86)\Steam\steamapps\common\The Binding of Isaac Rebirth") is { } path86)
            yield return path86;
    }

    public Process? LaunchIsaacGame(DirectoryInfo directory) {
        var isaacNgFiles = directory.GetFiles("isaac-ng.exe", SearchOption.TopDirectoryOnly);
        var isaacNgFile = isaacNgFiles.Single();

        var asm = typeof(WindowsPlatform).Assembly;
        using var bootstrapperStream = asm.GetManifestResourceStream("Tomat.Testament.Windows.natives.testament_windows_bootstrapper.dll");
        var bootstrapPath = Path.Combine(GetLocalApplicationDataDirectory().FullName, "natives", "bootstrapper.dll");
        Directory.CreateDirectory(Path.GetDirectoryName(bootstrapPath)!);
        bootstrapperStream!.CopyTo(File.OpenWrite(bootstrapPath));
        Console.WriteLine($"Wrote bootstrapper to '{bootstrapPath}'.");

        var proc = StartProgram(
            isaacNgFile.FullName,
            options: new ProcessStartOptions {
                StartFrozen = false,
            }
        );

        Console.WriteLine("Started process, injecting bootstrapper into isaac-ng.exe.");
        if (!InjectDll(proc!, bootstrapPath))
            Console.WriteLine("Failed to inject bootstrapper into isaac-ng.exe.");

        return proc;
    }
}
