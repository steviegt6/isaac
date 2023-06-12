using System;
using System.Runtime.InteropServices;

namespace Tomat.Testament;

public readonly struct InitArgs { }

public static class Native {
    [UnmanagedCallersOnly]
    public static unsafe void Initialize(InitArgs args) {
        Console.WriteLine("Initialized called! (C#)");
    }
}
