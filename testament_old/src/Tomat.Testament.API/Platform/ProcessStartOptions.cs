namespace Tomat.Testament.API.Platform;

/// <summary>
///     Options for starting a process.
/// </summary>
/// <param name="StartFrozen">Whether this program should start frozen.</param>
public sealed class ProcessStartOptions {
    public bool StartFrozen { get; set; } = true;
}
