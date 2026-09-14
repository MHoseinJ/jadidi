---@meta
-- Jadidi Engine API Declarations
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
---The rotation of the object in degrees (clockwise)
---@field rotation number
Transform = {}

---@class Sprite : Component
---The file path to the texture asset
---@field path string
---The rendering order (higher values draw on top)
---@field zIndex integer
---The color of the sprite (used when no texture is set)
---@field color Color
---Whether the sprite has a valid texture loaded
---@field hasTexture boolean
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
---Whether the animation is currently playing
---@field playing boolean
---The playback speed of the animation
---@field speed number
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
---The current velocity vector of the rigidbody.
---@field velocity Vector2
---The density of the body (affects mass).
---@field density number
---The friction of the body.
---@field friction number
---The type of body (0=Static, 1=Dynamic, 2=Kinematic)
---@field bodyType integer
Rigidbody = {}

---Apply an instantaneous impulse to the rigidbody
---@param impulse Vector2 The impulse vector to apply
function Rigidbody:applyImpulse(impulse) end

---Sets the velocity of the rigidbody.
---@param velocity Vector2 The velocity vector to set
function Rigidbody:setVelocity(velocity) end

---Sets the density of the rigidbody.
---@param density number The density value to set
function Rigidbody:setDensity(density) end

---Sets the friction of the rigidbody.
---@param friction number The friction value to set
function Rigidbody:setFriction(friction) end

---Sets the body type of the rigidbody.
---@param type integer The body type (0=Static, 1=Dynamic, 2=Kinematic)
function Rigidbody:setBodyType(type) end

---@class Button : Component
---@field zOrder integer
Button = {}

---add_function function
---can give a lua function and when user did mouse action the function called
---@param luaFunc function this is the lua function for input
---@param mouseButton integer this is the mouse action (e.g., Mouse.LEFT)
function Button:addFunction(luaFunc, mouseButton) end

---@class BoxCollider : Component
---Size of the collider in world units.
---@field size Vector2
---@field isTrigger boolean Whether this collider acts as a trigger (no physical collision)
BoxCollider = {}

---Rebuilds the physics body (call after changing size)
function BoxCollider:rebuildBody() end

---Checks whether this collider overlaps another BoxCollider.
---@param other BoxCollider
---@return boolean
function BoxCollider:overlap(other) end

---Checks whether a world-space point is inside this collider.
---@param point Vector2
---@return boolean
function BoxCollider:overlap(point) end

---@class Audio : Component
---get name of audio that is playing
---@field name string
---set and get the spatial boolean
---@field spatial boolean
---set and get the value of maxDistance
---@field maxDistance number
---get the channel of audio playing on
---@field channel integer
---get and set the number of loops
---@field loops integer
Audio = {}

---Play function
---@param name string The registered name of the sound
---@param loop number Count of cycles to play (-1 for infinite cycles)
function Audio:Play(name, loop) end

---Stop function
function Audio:Stop() end

---Get current volume (0-128)
---@return integer
function Audio:GetVolume() end

---Set volume (0-128)
---@param volume integer
function Audio:SetVolume(volume) end

---@class GameObject
---@field id integer
---@field name string
---@field tag string
---@field transform Transform
---@field valid boolean
---@field onCollisionEnter fun(other: GameObject)|nil
---@field onCollisionExit fun(other: GameObject)|nil
---@field onTriggerEnter fun(other: GameObject)|nil
---@field onTriggerExit fun(other: GameObject)|nil
GameObject = {}

---@param name string The type name of the component
---@return Component|nil
function GameObject:addComponent(name) end

---@param name string The type name of the component
---@return Component|nil
function GameObject:getComponent(name) end

function GameObject:destroy() end

---@return boolean
function GameObject:isValid() end

---@class RaycastHit
---@field hit boolean
---@field point Vector2
---@field normal Vector2
---@field fraction number
RaycastHit = {}

---@class Physics
Physics = {}

---@param start Vector2
---@param endpos Vector2
---@return RaycastHit
function Physics.raycast(start, endpos) end

---@param gravity Vector2
function Physics.setGravity(gravity) end