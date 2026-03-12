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

---Plays an animation with the specified name.
---(Only works if the component is an Animator)
---@param name string The name of the animation to play
function Component:Play(name) end

---Pauses the current animation.
---(Only works if the component is an Animator)
function Component:Pause() end

---Resumes the paused animation.
---(Only works if the component is an Animator)
function Component:Resume() end

---Stops the current animation.
---(Only works if the component is an Animator)
function Component:Stop() end

---Sets the playback speed of the animation.
---(Only works if the component is an Animator)
---@param s number The speed multiplier
function Component:SetSpeed(s) end

---Gets or sets the Z-index (rendering order) of the component.
---(Only works if the component is a Sprite)
---@field zIndex integer

---Gets or sets the file path of the sprite texture.
---(Only works if the component is a Sprite)
---@field path string

---Gets the velocity vector of the component.
---(Only works if the component is a Rigidbody)
---Note: This is a read-only property in this binding (getter only).
---@field velocity Vector2

---Reloads the sprite (calls OnCreate internally).
---(Only works if the component is a Sprite)
function Component:reload() end


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
---@field z_index integer
Sprite = {}

---Reloads the sprite texture.
---This releases the current texture and loads it again based on the path.
function Sprite:reload() end


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