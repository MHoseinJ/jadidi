---@meta
-- Jadidi Engine API Declarations
-- This file is for IDE autocomplete & type checking only (joke!)

------------------------------
----        LOGGER        ----
------------------------------

---@class Log
Log = {}

function Log.clear() end

---@param str string
function Log.print(str) end

---@param str string
function Log.info(str) end

---@param str string
function Log.warn(str) end

---@param str string
function Log.error(str) end