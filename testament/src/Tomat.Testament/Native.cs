/* This file is part of Tomat.Testament, a part of the Testament Project hosted
 * within the mono-repository located at <https://github.com/steviegt6/isaac>.
 * 
 * Tomat.Testament is released under the GNU General Public License, version 3.
 * As such, it is subject to the terms written in the `LICENSE-GPL` file located
 * within the project root.
 */

using System;
using System.Runtime.InteropServices;
using JetBrains.Annotations;

namespace Tomat.Testament;

internal static class Native {
    [UnmanagedCallersOnly]
    [UsedImplicitly(ImplicitUseKindFlags.Access)]
    internal static void Initialize() {
        Console.WriteLine("Initialize called! (C#)");
    }
}
