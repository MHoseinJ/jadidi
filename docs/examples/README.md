# Examples

Examples for Jadidi are written against a real game, not against a synthetic test scene. That game is still being built. This page is a placeholder so the link map stays accurate; it will be filled in with runnable walkthroughs once the reference game is complete.

The planned examples are:

| Page | Topic |
|------|-------|
| `first-game.md` | A complete, runnable small game, from project layout to a win condition |
| `game-objects.md` | Creating, finding, and destroying objects from Lua |
| `components.md` | Adding, reading, and configuring components from Lua |
| `input.md` | Keyboard and mouse input, edge-triggered and held-key queries |
| `movement.md` | Moving objects, mixing transforms and physics |
| `scenes.md` | Loading, switching, and structuring scenes |

## Starting point

The base template scene is a valid starting point for any of the planned examples:

    {
      "physics": {
        "gravity": {
          "x": 0.0,
          "y": 0.0
        }
      },
      "objects": []
    }

It creates an empty scene with gravity disabled. Add objects under `objects` and scripts under `Scripts/` as the examples describe. See [Scene files](../formats/scene.md) for the object and component schemas.

## Until then

While this section is being written, the closest material is:

- [Getting Started](../getting-started.md) — project layout and a minimal running scene
- [Lifecycle](../api/lifecycle.md) — script hooks and the main loop
- [Known Limitations](../limitations.md) — behaviors to design around