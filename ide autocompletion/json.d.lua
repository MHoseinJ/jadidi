---@meta
-- JadidCore Engine API Declarations
-- This file is for IDE autocomplete & type checking only (joke!)

------------------------------
----         JSON         ----
------------------------------

---@class Json
local Json = {}

---Reads a JSON file and converts it to a Lua table.
---@param path string The file path to the JSON file
---@return table|nil The parsed data as a Lua table, or nil if the file does not exist
function Json.read(path) end

---Writes a Lua table to a JSON file.
---@param path string The file path where the JSON should be saved
---@param table table The Lua table data to convert and write
---@return boolean True if the write operation was successful, false otherwise
function Json.write(path, table) end