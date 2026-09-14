---@meta
-- Jadidi Engine API Declarations
-- This file is for IDE autocomplete & type checking only (joke!)

------------------------------
----         JSON         ----
------------------------------

---@class Json
local Json = {}

---@param path string
---@return table|nil
function Json.read(path) end

---@param path string
---@param table table
---@return boolean
function Json.write(path, table) end