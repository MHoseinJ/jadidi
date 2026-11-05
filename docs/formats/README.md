# Formats

Jadidi uses JSON for configuration, scenes, and animations. This section documents each format.

- [config.json](config.md) — engine window and startup configuration
- [Scene files](scene.md) — `Scenes/<name>.json`
- [Animation files](animation.md) — `Animations/<name>.json`

## JSON parsing rules

All JSON is parsed with nlohmann/json, then wrapped in the engine's `Json` helper (`utils/Json.h`). The helper imposes these rules:

- Missing fields return the default value provided by the caller; they are not errors unless the caller uses `getRequired`.
- Type mismatches throw `JsonException`. The message includes the field's context path, for example `Player -> sprite: Type mismatch in field 'texture'. Expected string, but got integer`.
- `Vector2` fields accept either a two-element array `[x, y]` or an object `{ "x": ..., "y": ... }`.
- A nested object is opened with `getObject(field)` and an array with `getArray(field)`.
- Invalid or missing files are detected via `Json::isValid()`; callers decide how to react.