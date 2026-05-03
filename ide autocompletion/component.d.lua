---@meta
-- JadidCore Engine API Declarations
-- This file is for IDE autocomplete & type checking only (joke!)

------------------------------
----   COMPONENT SYSTEM   ----
------------------------------

---@class Component
---The GameObject that owns this component
---@field owner GameObject
Component = {}

---@class Transform : Component
---The position of the object in 2D space
---@field position Vector2
---The scale of the object (default is 1.0, 1.0)
---@field scale Vector2
Transform = {}

---@class Sprite : Component
---The file path to the texture asset
---@field path string
---The rendering order (higher values draw on top)
---@field zIndex integer
Sprite = {}

---Reloads the sprite texture.
---This releases the current texture and loads it again based on the path.
function Sprite:reload() end

---@class Text : Component
---The text that you want to show
---@field text string
---font size of the text
---@field fontSize integer
---name of the font that you want
---@field fontName string
---the color of text
---@field color Color

Text = {}

---Reloads the Text component
---this is necessary when you created a text component and valued them and reload it to use it
function Text:reload() end

---get size of text texture
---@return Vector2
function Text:size() end


---@class Animator : Component
Animator = {}

---Plays the specified animation.
---@param name string The name of the animation to play
---@param restart? boolean Whether to restart the animation if it is already playing (default: false)
function Animator:Play(name, restart) end

---Pauses the current animation.
function Animator:Pause() end

---Resumes the paused animation.
function Animator:Resume() end

---Stops the current animation and resets it.
function Animator:Stop() end

---Sets the playback speed of the animation.
---@param s number The speed multiplier
function Animator:SetSpeed(s) end


---@class Rigidbody : Component
---The velocity vector of the rigidbody
---@field velocity Vector2
Rigidbody = {}


---@class GameObject
---The unique ID of the game object
---@field id integer
---The name of the game object
---@field name string
---The Transform component attached to this object
---@field transform Transform
GameObject = {}

---Adds a component of the specified type to this game object.
---@param name string The type name of the component (e.g., "Sprite", "Animator")
---@return Component The newly created component
function GameObject:addComponent(name) end

---Gets a component of the specified type attached to this game object.
---@param name string The type name of the component (e.g., "Sprite", "Animator")
---@return Component The component if found, otherwise nil
function GameObject:getComponent(name) end


---@class Button : Component
---add_function function
---can give a lua function and when user did mouse action the function called
---USAGE: Button:addFunction(myFunc, Mouse.LEFT)
---@param luaFunc function this is the lua function for input
---@param mouseButton Mouse this is the mouse action. you can also use integer like 0 for LEFT and 1 for MIDDLE and 2 FOR RIGHT
function Button:addFunction(luaFunc, mouseButton) end
---@return void
---@field zOrder integer
Button = {}

---@class BoxCollider
---the size of collider
---size param in a Vector2
---@field size Vector2
BoxCollider = {}

---@class Audio
---get name of audio that is playing
---@field name string
---set and get the spatial boolean
---@field spatial boolean
---set and get the value of maxDistance
---@field maxDistance number
---get the chanel of audio playing on
---@field chanel number
---Play function
---you have to pass the below parameters
---@param name string name of the sound that loaded with it using AudioSystem:load(name, path, isMusic)
---@param loop number count of cycles to play ( -1 for infinit cycles )

function Audio:Play(path, loop) end
---@return void

---Stop function
function Audio:Stop() end
---@return void

Audio = {}