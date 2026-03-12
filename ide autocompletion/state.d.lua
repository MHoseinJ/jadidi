---@meta
-- JadidCore Engine API Declarations
-- This file is for IDE autocomplete & type checking only (joke!)

------------------------------
----     STATE SYSTEM     ----
------------------------------

---@class State
State = {}

---Sets a value in the global state with the given key.
---@param key string The key to identify the value
---@param value any The value to store (can be number, string, table, etc.)
function State.set(key, value) end

---Gets a value from the global state by key.
---@param key string The key to retrieve
---@return any|nil The stored value, or nil if the key does not exist
function State.get(key) end

---Checks if a key exists in the global state.
---@param key string The key to check
---@return boolean True if the key exists, false otherwise
function State.exists(key) end

---Removes a key-value pair from the global state.
---@param key string The key to remove
function State.remove(key) end

---Clears all data from the global state.
function State.clear() end