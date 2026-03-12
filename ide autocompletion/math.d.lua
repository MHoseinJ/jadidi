---@meta
-- JadidCore Engine API Declarations
-- This file is for IDE autocomplete & type checking only (joke!)

------------------------------
----         MATH         ----
------------------------------

---@class Vector2
--- x axis field
---@field x number
--- y axis field
---@field y number
Vector2 = {}

--- empty construction
---@return Vector2
function Vector2.new() end

--- constructor with fields
---@param x number x axis
---@param y number y axis
---@return Vector2
function Vector2.new(x, y) end

--- setter
---@param pos Vector2 change whole vector
function Vector2:set(pos) end

--- move vector
---@param diff Vector2 adds vector or move it
function Vector2:move(diff) end

--- adder
---@param vecToAdd Vector2 vector to add
---@return Vector2 returns new vector
function Vector2:__add(vecToAdd) end

--- multiplier
---@param scalar number multiplier number
---@return Vector2 returns new vector
function Vector2:__mul(scalar) end

return Vector2