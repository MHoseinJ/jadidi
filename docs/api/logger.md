# Logger

This page is a hub for the logging API.

## What you need to know

Logging is exposed through the `Log` global. `Log.print` logs a plain message; `Log.info`, `Log.warn`, and `Log.error` log at increasing severities. `Log.clear` clears the in-game log.

Log messages are shown in the in-game log overlay when `DEV_MODE` is enabled, and appear on stderr for engine-internal warnings and errors. Lua errors raised by a script are caught by the engine and logged through the same channel, with the script path in the message.

## Where each part lives

| What | Where |
|------|-------|
| `Log.print`, `Log.info`, `Log.warn`, `Log.error`, `Log.clear` | [Engine Overview → Log](engine.md#log) |
| Error handling behavior for scripts | [Lifecycle → Error handling](lifecycle.md#error-handling) |
| Severity and where logs are displayed | [Lifecycle → Main loop](lifecycle.md#2-run--main-loop) (`DEV_MODE` overlay) |

## Typical usage

    Log.print("Hello")
    Log.info("Loaded " .. count .. " objects")
    Log.warn("No font specified, using default")
    Log.error("Player not found")

Errors that come from a C++ component (`BoxCollider`, `Rigidbody`, etc.) are written to stderr by the component itself and may not appear in the in-game log; see the per-component notes in [Components](components.md) if a behavior seems inconsistent with the in-game log.