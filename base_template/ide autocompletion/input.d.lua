---@meta
-- Jadidi Engine API Declarations
-- This file is for IDE autocomplete & type checking only (joke!)

------------------------------
----        INPUT         ----
------------------------------

---@class Input
Input = {}

---@param scancode integer
---@return boolean
function Input.keyPressed(scancode) end

---@param scancode integer
---@return boolean
function Input.keyDown(scancode) end

---@param scancode integer
---@return boolean
function Input.keyUp(scancode) end

---@param button integer
---@return boolean
function Input.mousePressed(button) end

---@param button integer
---@return boolean
function Input.mouseDown(button) end

---@param button integer
---@return boolean
function Input.mouseUp(button) end