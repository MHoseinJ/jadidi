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

--- calculates distance between this vector and another
---@param other Vector2 the other vector
---@return number the distance
function Vector2:distance(other) end

--- returns the magnitude (length) of the vector
---@return number the length
function Vector2:length() end

--- returns a new normalized vector (unit vector with same direction)
---@return Vector2 the normalized vector
function Vector2:normalized() end

--- returns the dot product of this vector and another
---@param other Vector2 the other vector
---@return number the dot product
function Vector2:dot(other) end

--- adder
---@param vecToAdd Vector2 vector to add
---@return Vector2 returns new vector
function Vector2:__add(vecToAdd) end

--- subtractor
---@param vecToSub Vector2 vector to subtract
---@return Vector2 returns new vector
function Vector2:__sub(vecToSub) end

--- multiplier
---@param scalar number multiplier number
---@return Vector2 returns new vector
function Vector2:__mul(scalar) end

--- divider
---@param scalar number divisor number
---@return Vector2 returns new vector
function Vector2:__div(scalar) end

--- returns the magnitude (length) of the vector
---@return number the length
function Vector2:length() end

--- returns a new normalized vector (unit vector with same direction)
---@return Vector2 the normalized vector
function Vector2:normalized() end

--- returns the dot product of this vector and another
---@param other Vector2 the other vector
---@return number the dot product
function Vector2:dot(other) end

---@class Color
---red value (0-255)
---@field r integer
---green value (0-255)
---@field g integer
---blue value (0-255)
---@field b integer
---alpha value (0-255)
---@field a integer
Color = {}

---Empty construction
---@return Color
function Color.new() end