---@meta
-- Jadidi Engine API Declarations
-- This file is for IDE autocomplete & type checking only (joke!)

------------------------------
----         MATH         ----
------------------------------

---@class Vector2
---@field x number
---@field y number
Vector2 = {}

---@return Vector2
function Vector2.new() end

---@param x number
---@param y number
---@return Vector2
function Vector2.new(x, y) end

---@param pos Vector2
function Vector2:set(pos) end

---@param diff Vector2
function Vector2:move(diff) end

---@param other Vector2
---@return number
function Vector2:distance(other) end

---@return number
function Vector2:length() end

---@return Vector2
function Vector2:normalized() end

---@param other Vector2
---@return number
function Vector2:dot(other) end

---@param vecToAdd Vector2
---@return Vector2
function Vector2:__add(vecToAdd) end

---@param vecToSub Vector2
---@return Vector2
function Vector2:__sub(vecToSub) end

---@param scalar number
---@return Vector2
function Vector2:__mul(scalar) end

---@param scalar number
---@return Vector2
function Vector2:__div(scalar) end

---@class Color
---@field r integer
---@field g integer
---@field b integer
---@field a integer
Color = {}

---@return Color
function Color.new() end