# Json

This page is a hub for the Lua JSON helpers.

## What you need to know

`Json.read(path)` reads a JSON file and converts it to a Lua table. `Json.write(path, table)` serializes a Lua table and writes it to a file. There is no general-purpose filesystem API — `Json` is the only file I/O exposed to Lua.

The conversion is bidirectional and follows fixed rules, so a value written by `Json.write` and read back with `Json.read` is structurally identical, with one caveat: JSON `null` becomes Lua `nil`, and Lua `nil` values in a table are skipped when writing (they do not become JSON `null`). Numeric keys that look like array indices are preserved; a Lua table whose keys are exactly `1..n` is written as a JSON array.

## Where each part lives

| What | Where |
|------|-------|
| `Json.read` and `Json.write` signatures | [Engine Overview → Json](engine.md#json) |
| The exact conversion rules (objects, arrays, numerics) | [Engine Overview → Json](engine.md#json) |
| The JSON parser used by the engine internally (not exposed to Lua) | [Formats → JSON parsing rules](../formats/README.md#json-parsing-rules) |
| Where `Json.read` is used inside the engine (scene files) | [Formats → Scene files](../formats/scene.md) |

## Typical usage

    local data = Json.read("saves/slot1.json")
    if data == nil then
        data = { level = 1, score = 0 }
    end

    data.score = data.score + 10
    Json.write("saves/slot1.json", data)

`Json.read` returns `nil` if the file does not exist; it does not throw for a missing file. Invalid JSON content, however, depends on the underlying parser and is not documented here as a stable behavior — treat parse errors as exceptional.