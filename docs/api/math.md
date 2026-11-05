# Math Types

Jadidi exposes a small set of math types to Lua. They are implemented as sol2 user types, so they support method calls and operator overloads.

## Vector2

A 2D vector of floats.

### Construction

    Vector2()                 -- (0, 0)
    Vector2(x, y)             -- (x, y)

### Fields

| Field | Type | Description |
|-------|------|-------------|
| `x` | number | X component |
| `y` | number | Y component |

Both fields are writable.

### Methods

| Method | Signature | Description |
|--------|-----------|-------------|
| `set` | `v:set(x, y)` | Sets both components |
| `setVec` | `v:setVec(other)` | Copies components from another `Vector2` |
| `move` | `v:move(dx, dy)` | Moves by a delta. Exact signature is `Vector2::move`; pass a `Vector2` or numeric components depending on the overload. |
| `distance` | `v:distance(other)` | Distance to another `Vector2` |
| `length` | `v:length()` | Length of the vector |
| `normalized` | `v:normalized()` | Returns a unit-length copy |
| `dot` | `v:dot(other)` | Dot product with another `Vector2` |

> If a method's exact overload set is important to you, check `utils/math/vector.h`. The list above reflects the bindings in `BindMath.cpp`.

### Operators

| Operator | Result |
|----------|--------|
| `a + b` | `Vector2` |
| `a - b` | `Vector2` |
| `a * number` | `Vector2` |
| `a / number` | `Vector2` |

The operators are bound to `Vector2`'s corresponding member operators. Vector + vector and vector - vector are supported; multiplication and division are by scalar.

## Color

An RGBA color. It is bound from `SDL_Color`, so it uses the SDL channel layout.

### Construction

    Color()                   -- RGBA (0, 0, 0, 0) — does not set defaults for you

> The binding only exposes the default constructor. `Color()` leaves the channels undefined; assign them before use.

### Fields

| Field | Type | Description |
|-------|------|-------------|
| `r` | number | Red (0–255) |
| `g` | number | Green (0–255) |
| `b` | number | Blue (0–255) |
| `a` | number | Alpha (0–255) |

All fields are writable.

## SDL_Rect

A rectangle with integer fields.

### Fields

| Field | Type | Description |
|-------|------|-------------|
| `x` | number | Left position |
| `y` | number | Top position |
| `w` | number | Width |
| `h` | number | Height |

There is no constructor exposed; `SDL_Rect` values are produced by engine APIs (for example, `Sprite.srcRect`) and can be modified in place.