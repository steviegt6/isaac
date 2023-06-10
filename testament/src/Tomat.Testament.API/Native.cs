using System.Runtime.InteropServices;

namespace Tomat.Testament.API;

public readonly struct InitArgs { }

public static class Native {
    [UnmanagedCallersOnly]
    public static unsafe void Initialize(InitArgs* args) { }
}
