---@meta
-- Jadidi Engine API Declarations
-- This file is for IDE autocomplete & type checking only (joke!)

---Initializes the engine (called internally or at startup).
function init() end

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