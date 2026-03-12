---@meta
-- JadidCore Engine API Declarations
-- This file is for IDE autocomplete & type checking only (joke!)

------------------------------
----        LOGGER        ----
------------------------------

---@class Log
local Log = {}

---Clears the log output.
function Log.clear() end

---Prints a general message to the log.
---@param str string The message to print
function Log.print(str) end

---Logs an informational message.
---@param str string The message to log
function Log.info(str) end

---Logs a warning message.
---@param str string The warning message to log
function Log.warn(str) end

---Logs an error message.
---@param str string The error message to log
function Log.error(str) end