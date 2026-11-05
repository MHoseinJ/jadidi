# State

This page is a hub for the global state table.

## What you need to know

`State` is a global in-memory key-value store shared across all scripts. Keys are strings; values can be any Lua value. It is the natural place for cross-scene data (score, settings, references to objects that should survive a scene switch) — except that a scene switch destroys all objects, so object handles stored in `State` become invalid and `valid` will be `false` afterward.

`State` does not persist to disk. If you need persistence, serialize with `Json.write` and reload with `Json.read`.

## Where each part lives

| What | Where |
|------|-------|
| `State.set`, `get`, `exists`, `remove`, `clear` | [Engine Overview → State](engine.md#state) |
| How `State` stores values | [Known Limitations → State](../limitations.md#state-stores-solobject-references) |
| `Json.read` / `Json.write` for persistence | [Engine Overview → Json](engine.md#json) |

## Typical usage

    -- On first run, initialize
    if not State.exists("score") then
        State.set("score", 0)
    end

    -- Update score
    State.set("score", State.get("score") + 10)

    -- Read from another script
    local score = State.get("score")

`State.set` stores tables by reference. If two scripts share a table via `State`, mutations are visible to both. Store primitive values or serialize with `Json` if you need snapshots.