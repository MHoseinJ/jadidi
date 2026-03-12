---@meta
-- JadidCore Engine API Declarations
-- This file is for IDE autocomplete & type checking only (joke!)

------------------------------
----        INPUT         ----
------------------------------

---@class Input
Input = {}

---Checks if a key was just pressed (triggered once per press).
---@param scancode integer The scancode of the key
---@return boolean True if the key was just pressed
function Input.keyPressed(scancode) end

---Checks if a key is currently held down.
---@param scancode integer The scancode of the key
---@return boolean True if the key is down
function Input.keyDown(scancode) end

---Checks if a key was just released.
---@param scancode integer The scancode of the key
---@return boolean True if the key was just released
function Input.keyUp(scancode) end

---Checks if a mouse button was just pressed (triggered once per click).
---@param button integer The button code (e.g., 1 for left, 2 for middle, 3 for right)
---@return boolean True if the button was just pressed
function Input.mousePressed(button) end

---Checks if a mouse button is currently held down.
---@param button integer The button code
---@return boolean True if the button is down
function Input.mouseDown(button) end

---Checks if a mouse button was just released.
---@param button integer The button code
---@return boolean True if the button was just released
function Input.mouseUp(button) end