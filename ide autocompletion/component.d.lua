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
function Sprite:reload() end

---Get the size of the current source rectangle.
---@return Vector2
function Sprite:size() end

---The source rectangle for rendering (used for sprite sheets).
---Allows you to define which part of the texture to draw.
---@class SDL_Rect
---@field x integer The x position of the rectangle
---@field y integer The y position of the rectangle
---@field w integer The width of the rectangle
---@field h integer The height of the rectangle
Sprite.srcRect = {}

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
---@param loops integer The count of cycles to play animation (infinite)
function Animator:Play(name, loops) end

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
---Whether the body is dynamic (affected by gravity/forces) or static (immovable)
---@field isDynamic boolean
---The density of the body (affects mass calculation)
---@field density number
---The friction of the body's surface (0.0 to 1.0+)
---@field friction number
---The velocity vector. 
---Note: Modifying this from Lua may not affect the Box2D simulation directly 
---unless you implement a custom setter in C++ that updates the b2BodyId.
---@field velocity Vector2|nil
Rigidbody = {}

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

---@class BoxCollider : Component
---Size of the collider in world units.
---@field size Vector2
BoxCollider = {}

---Checks whether this collider overlaps another BoxCollider.
---@param other BoxCollider
---@return boolean
function BoxCollider:overlap(other) end

---Checks whether a world-space point is inside this collider.
---@param point Vector2
---@return boolean
function BoxCollider:overlap(point) end

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
---Plays a sound that was previously loaded using AudioSystem.load(name, path, isMusic)
---@param name string The registered name of the sound (NOT the file path)
---@param loop number Count of cycles to play (-1 for infinite cycles)
function Audio:Play(name, loop) end
---@return void

---Stop function
function Audio:Stop() end
---@return void

Audio = {}

---@class GameObject
---The unique ID of the game object
---@field id integer
---The name of the game object
---@field name string
---The tag of the game object
---@field tag string
---The Transform component attached to this object
---@field transform Transform
---Check if this handle is still pointing to a valid object
---@field valid boolean
GameObject = {}

---Adds a component of the specified type to this game object.
---@param name string The type name of the component (e.g., "Sprite", "Animator")
---@return Component|nil The newly created component
function GameObject:addComponent(name) end

---Gets a component of the specified type attached to this game object.
---@param name string The type name of the component (e.g., "Sprite", "Animator")
---@return Component|nil The component if found, otherwise nil
function GameObject:getComponent(name) end

---Destroys this game object immediately.
function GameObject:destroy() end

---Checks if the object is valid.
---@return boolean
function GameObject:isValid() end