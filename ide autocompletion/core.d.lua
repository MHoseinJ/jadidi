---@meta
-- JadidCore Engine API Declarations
-- This file is for IDE autocomplete & type checking only (joke!)

---The name of the scene this script belongs to.
---This variable is required for the script to be loaded in the specific scene.
---@type string
RUN_IN_SCENE = ""

---Called once when the object is created or the scene starts.
---This function is optional.
function start() end

---Called every frame.
---This function is optional.
---@param dt number The delta time in seconds
function update(dt) end