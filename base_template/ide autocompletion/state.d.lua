---@meta
-- Jadidi Engine API Declarations
-- This file is for IDE autocomplete & type checking only (joke!)

------------------------------
----     STATE SYSTEM     ----
------------------------------

---@class State
State = {}

---@param key string
---@param value any
function State.set(key, value) end

---@param key string
---@return any|nil
function State.get(key) end

---@param key string
---@return boolean
function State.exists(key) end

---@param key string
function State.remove(key) end

function State.clear() end