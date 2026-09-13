---@meta
-- Jadidi Engine API Declarations
-- This file is for IDE autocomplete & type checking only (joke!)

---Defines in which scene(s) this script should be loaded.
---
---Behavior:
---  - If omitted (nil) -> Script is GLOBAL and loaded in ALL scenes.
---  - If a string      -> Script loads ONLY in that specific scene.
---  - If an array      -> Script loads in ALL specified scenes.
---
---@type string|string[]|nil
RUN_IN_SCENE = ""

---Called once when the object is created or the scene starts.
---This function is optional.
function start() end

---Called every frame.
---This function is optional.
---@param dt number The delta time in seconds
function update(dt) end

---screen engine api for getting things like screen size
---@class Screen
Screen = {}
---get size of the screen with a vector2
---@return Vector2
function Screen.size() end