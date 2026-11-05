# Components

Components are attached to a `GameObject` and run every frame through `GameObject::Update(dt)`, which calls `Update(dt)` on each component in the object's component map.

This page documents every component exposed to Lua, the base `Component` user type, and the component names accepted by `obj:addComponent(name)` / `obj:getComponent(name)`.

## Component names

| Name | C++ type |
|------|----------|
| `sprite` | `Sprite` |
| `animator` | `Animator` |
| `text` | `Text` |
| `button` | `Button` |
| `audio` | `Audio` |
| `boxCollider` | `BoxCollider` |
| `rigidbody` | `Rigidbody` |
| `lua:<path>` | `LuaComponent` |
| `transform` | `Transform` (always present; cannot be added) |

`addComponent("lua:<path>")` is the dynamic equivalent of the `"lua:<path>"` key in a scene file.

## Component base

All components share a common user type named `Component` in Lua. Its members apply to every component instance, but many of them only work on specific component types; using them on the wrong type raises a Lua error.

### Members

| Member | Kind | Works on | Description |
|--------|------|----------|-------------|
| `Play(name, loops?)` | method | `Animator`, `Audio` | For `Animator`, plays the named animation. For `Audio`, plays the named sound with `loops` (default `0` for `Animator`, `0` for `Audio` in this binding — see notes). Raises if the component is neither. |
| `Pause()` | method | `Animator` | Pauses the animation. Raises on other components. |
| `Resume()` | method | `Animator` | Resumes a paused animation. Raises on other components. |
| `Stop()` | method | `Animator`, `Audio` | Stops the animation or sound. Raises on other components. |
| `SetSpeed(speed)` | method | `Animator` | Sets playback speed. Raises on other components. |
| `zIndex` | property (int) | `Sprite` | Sprite draw order. Assigning marks the renderer dirty. Raises on other components. |
| `path` | property (string) | `Sprite` | Texture path. Assigning reloads the texture. Raises on other components. |
| `reload()` | method | `Sprite`, `Text` | Reloads the underlying texture. Raises on other components. |
| `srcRect` | property (`SDL_Rect`) | `Sprite` | Source rectangle within the texture. Raises on other components. |
| `color` | property (`Color`) | `Sprite`, `Text` | Tint color. Assigning to `Text.color` triggers a reload. Raises on other components. |
| `velocity` | property (`Vector2`) | `Rigidbody` | Body velocity. Raises on other components. |
| `density` | property (number) | `Rigidbody` | Body density. Raises on other components. |
| `friction` | property (number) | `Rigidbody` | Body friction. Raises on other components. |
| `applyImpulse(impulse)` | method | `Rigidbody` | Applies an impulse. Raises on other components. |
| `bodyType` | property (int) | `Rigidbody` | `0` = static, `1` = kinematic, `2` = dynamic. Raises on other components. |
| `size` | property (`Vector2`) | `Sprite`, `Text`, `BoxCollider` | For `Sprite`/`Text`, read-only effective size. For `BoxCollider`, read/write; assigning rebuilds the body. Raises on other components. |
| `text` | property (string) | `Text` | Text content. Assigning triggers a reload. Raises on other components. |
| `fontName` | property (string) | `Text` | Font file name. Assigning triggers a reload. Raises on other components. |
| `fontSize` | property (int) | `Text` | Font size in pixels. Assigning triggers a reload. Raises on other components. |
| `addFunction(fn, mouse)` | method | `Button` | Registers `fn` for a mouse button. Raises on other components. |
| `zOrder` | property (int) | `Button` | Button ordering. Raises on other components. |
| `name` | property (string) | `Audio` | Sound name. Raises on other components. |
| `spatial` | property (bool) | `Audio` | Whether the sound is spatialized. Raises on other components. |
| `maxDistance` | property (number) | `Audio` | Maximum audible distance. Raises on other components. |
| `volume` | property (int) | `Audio` | Volume. Reads via `GetVolume`, writes via `SetVolume`. Raises on other components. |
| `loops` | property (int) | `Audio` | Number of loops. Raises on other components. |
| `overlap(other)` | method | `BoxCollider` | Overlap test against another `BoxCollider` or a `Vector2` point. Raises on other components. |

### Notes on the generic methods

- `Component.Play(name, loops)` — the binding signature is `(name, loops = 0)`. For `Animator`, only `name` is used; for `Audio`, `name` and `loops`.
- The generic `Component` user type exists so a script can call methods like `comp:Stop()` on a component obtained from `getComponent`, without knowing its concrete type. It throws a Lua error if the component is not of a compatible type.
- Components have their own user types (`Sprite`, `Animator`, etc.). Those bind a subset of these members again for clarity and to expose type-specific fields (for example, `Animator.playing` and `Animator.speed`).

## Sprite

Draws a texture (or nothing, if `path` is empty, in which case the engine still reserves a `srcRect` — default 16×16).

### Properties

| Property | Type | Access | Description |
|----------|------|--------|-------------|
| `path` | string | read/write | Texture path. Assigning calls `SetPath`, which releases and reacquires the texture |
| `zIndex` | int | read/write | Draw order |
| `hasTexture` | bool | read-only | Whether a texture is currently loaded |
| `color` | `Color` | read/write | Tint |
| `srcRect` | `SDL_Rect` | read/write | Source rectangle within the texture |

### Methods

| Method | Signature | Description |
|--------|-----------|-------------|
| `size` | `sprite:size()` | Returns a `Vector2` with `srcRect.w` and `srcRect.h` |
| `reload` | `sprite:reload()` | Releases and reacquires the texture |

### Behavior

- `OnCreate` loads the texture from `path` (if set) and sets `srcRect` to the texture's full size, or `{0, 0, 16, 16}` if there is no texture.
- Assigning `path` to a different value triggers a release-and-load cycle.
- `srcRect` does not clamp; set it directly to crop a sprite sheet.

## Animator

Plays named animations on a sibling `Sprite`. The `Animator` looks for a `Sprite` on the same `GameObject`.

Animations are loaded from `Animations/<name>.json` the first time they are played, and cached by name. The JSON schema is documented in [Formats — animation](../formats/animation.md).

### Properties

| Property | Type | Access | Description |
|----------|------|--------|-------------|
| `playing` | bool | read/write | Assigning `true` calls `Resume`, `false` calls `Pause` |
| `speed` | number | read/write | Playback speed. Negative values are clamped to `0` |

### Methods

| Method | Signature | Description |
|--------|-----------|-------------|
| `Play` | `animator:Play(name [, restart])` | Plays the named animation. If `name` equals the current animation and `restart` is false, this is a no-op. With no `restart`, an already-playing animation is not restarted |
| `Pause` | `animator:Pause()` | Stops advancing frames |
| `Resume` | `animator:Resume()` | Resumes advancing frames |
| `Stop` | `animator:Stop()` | Stops and clears the current animation |
| `SetSpeed` | `animator:SetSpeed(speed)` | Sets playback speed, clamped to `>= 0` |

### Behavior

- `Animator` requires a `Sprite` sibling. Without one, `Update` returns early.
- The first `Update` call after creation initializes the animator: if the deserialized `play` field is non-empty, that animation starts.
- The animation's `loop` field controls what happens at the last frame. When `loop` is `false`, the last frame stops playback.
- The `srcRect` of the sibling `Sprite` is updated on each frame to select the current frame of the sprite sheet.

## Audio

Plays a named sound loaded through `AudioSystem.load`. `Audio` itself does not load files; it asks `AudioSystem` to play an already-loaded name.

### Properties

| Property | Type | Access | Description |
|----------|------|--------|-------------|
| `name` | string | read/write | Sound name (as loaded through `AudioSystem.load`) |
| `spatial` | bool | read/write | Enables spatial attenuation each frame |
| `maxDistance` | number | read/write | Maximum audible distance when `spatial` is enabled. Default `5000` |
| `channel` | int | read/write | Channel index. Deserialized from JSON. It is not updated by `Play` in the current code |
| `loops` | int | read/write | Loop count. Default `1` |
| `volume` | int | read/write | Volume (0–128). Reads via `GetVolume`, writes via `SetVolume` |

### Methods

| Method | Signature | Description |
|--------|-----------|-------------|
| `Play` | `audio:Play(name [, loop])` | Sets `name` and `loops`, then asks `AudioSystem` to play it |
| `Stop` | `audio:Stop()` | Stops the specific sound effect and clears `name` |

### Behavior

- When `spatial` is true, `Update` computes distance between the object's world position and the camera's world position, and scales volume accordingly. Outside `maxDistance`, volume is `0`.
- `Audio` only stops the sound effect by name; it does not use music APIs. Use `AudioSystem` for music streaming, and `Audio` for sound effects.
- There is a known issue in the current implementation where `channel` is only assigned on failure. Treat `channel` as a value deserialized from JSON; do not assume it reflects the actual playback channel after `Play`.

## Text

Renders a string using a font loaded by `FontManager`.

### Properties

| Property | Type | Access | Description |
|----------|------|--------|-------------|
| `text` | string | read/write | Text content. Assigning triggers a reload |
| `fontName` | string | read/write | Font name. Assigning triggers a reload |
| `fontSize` | int | read/write | Font size in pixels. Assigning triggers a reload |
| `color` | `Color` | read/write | Text color. Assigning triggers a reload |

### Methods

| Method | Signature | Description |
|--------|-----------|-------------|
| `size` | `text:size()` | Returns a `Vector2` with `srcRect.w` and `srcRect.h` |
| `reload` | `text:reload()` | Renders the text again |

### Behavior

- `OnCreate` renders the text into a texture if `text`, `fontName`, and `fontSize` are all set.
- Changing `text`, `fontName`, `fontSize`, or `color` triggers `Reload`. Reload calls `OnDestroy` then `OnCreate`, which destroys and re-creates the text texture.

## Button

A clickable area that invokes Lua functions registered with `addFunction`.

### Properties

| Property | Type | Access | Description |
|----------|------|--------|-------------|
| `zOrder` | int | read/write | Button ordering |

### Methods

| Method | Signature | Description |
|--------|-----------|-------------|
| `addFunction` | `button:addFunction(fn, mouse)` | Registers `fn` for the given mouse button constant (for example, `Mouse.LEFT`) |

### Behavior

- The button invokes the registered functions internally. There is no Lua method to call them; the engine calls them on a mouse event.
- Lua functions can also be attached through the scene file with a `functions` array. See [Formats — scene](../formats/scene.md).

## BoxCollider

Defines an axis-aligned box for physics and overlap tests.

### Properties

| Property | Type | Access | Description |
|----------|------|--------|-------------|
| `size` | `Vector2` | read/write | Collider size. Assigning rebuilds the physics body |
| `isTrigger` | bool | read/write | Whether the collider is a trigger |

### Methods

| Method | Signature | Description |
|--------|-----------|-------------|
| `rebuildBody` | `collider:rebuildBody()` | Recreates the physics body from the current size and trigger flag |
| `overlap` | `collider:overlap(other)` | Overlap test against another `BoxCollider` |
| `overlap` | `collider:overlap(point)` | Point-in-box test against a `Vector2` |

### Behavior

- If the `GameObject` has a `Rigidbody`, the collider does not own a separate physics body; the rigidbody uses the collider's size and trigger flag.
- If there is no `Rigidbody`, the collider creates its own static physics body on creation.
- Changing `size` in Lua rebuilds the body.

### Overlap test

`IsColliding` uses axis-aligned rectangles centered on the object's world position:

- Box vs box: overlapping if `|dx| < (wA + wB) / 2` and `|dy| < (hA + hB) / 2`.
- Point vs box: inside if `|dx| <= w / 2` and `|dy| <= h / 2`.

## Rigidbody

Gives a `GameObject` a physical body. A `Rigidbody` requires a `BoxCollider` on the same object to have a shape; without one, the body is created with zero size and `hasCollider = false`.

### Properties

| Property | Type | Access | Description |
|----------|------|--------|-------------|
| `bodyType` | int | read/write | `0` static, `1` kinematic, `2` dynamic |
| `density` | number | read/write | Body density. Assigning calls `setDensity` |
| `friction` | number | read/write | Body friction. Assigning calls `setFriction` |
| `velocity` | `Vector2` | read/write | Body velocity. Assigning calls `setVelocity` |

### Methods

| Method | Signature | Description |
|--------|-----------|-------------|
| `applyImpulse` | `rb:applyImpulse(impulse)` | Applies an impulse (`Vector2`) |
| `setVelocity` | `rb:setVelocity(v)` | Sets velocity |
| `setDensity` | `rb:setDensity(d)` | Sets density |
| `setFriction` | `rb:setFriction(f)` | Sets friction |
| `setBodyType` | `rb:setBodyType(type)` | Sets body type |


### Behavior

- `OnCreate` checks that a `BoxCollider` exists and, if so, removes the collider's standalone physics body. The `Rigidbody` then creates the body.
- `Update` reads position and velocity from the physics engine and writes position to the `Transform`. Scripts that assign `owner.transform.position` will fight the physics engine; use `velocity` or `applyImpulse` instead.
- **Dynamic bodies cannot be attached to child objects.** If `bodyType` is `Dynamic` (`2`) and the owner has a parent, the engine logs to `std::cerr`, does not create the physics body, and returns from `OnCreate`. The component is still attached, but `Update` will call into the physics engine with an invalid body handle. **The engine aborts on the next frame** with a Box2D assertion (`b2Body_IsValid`, usually `SIGILL`). `Static` and `Kinematic` bodies are not restricted.
- The check runs only in `OnCreate`. Calling `setBodyType(Dynamic)` on a child later does not re-trigger it; it forwards to the physics engine. Because the child's body was never created, this will also lead to invalid-body use. Do not use dynamic bodies on children, whether via JSON or via script.

## LuaComponent

A `LuaComponent` is created from `"lua:<path>"`. It is not obtained by `getComponent("lua:...")` — `getComponent` takes a component name and the `LuaComponent`'s `typeName()` is `"lua:<path>"`. To find it, iterate the components or query by the exact path.

See [Lifecycle](../api/lifecycle.md) for the script hooks and error handling.

## See also

- [Objects, Transform, and Camera](objects.md)
- [Engine Overview](engine.md)
- [Formats](../formats/README.md)