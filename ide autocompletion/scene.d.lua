---@meta
-- JadidCore Engine API Declarations
-- This file is for IDE autocomplete & type checking only (joke!)

------------------------------
----         ENGINE       ----
------------------------------

---@class Engine
Engine = {}

---Exits the game engine gracefully.
function Engine.exit() end

------------------------------
----    GLOBAL HELPERS    ----
------------------------------

---Checks if a GameObject handle is still valid (object exists).
---@param obj GameObject|nil The object handle to check
---@return boolean True if the object exists, false otherwise
function isValid(obj) end

------------------------------
----    SCENE  MANAGER    ----
------------------------------

---@class Scene
Scene = {}

---Loads a scene by its name.
---@param name string The name of the scene to load
function Scene.load(name) end

---@class Objects
Objects = {}

---Finds a game object by its name or ID.
---Returns a GameObject handle, or nil if not found.
---@param name string The name of the object to find
---@return GameObject|nil
function Objects.find(name) end

---@overload fun(id: integer): GameObject|nil
function Objects.find(id) end

---Creates a new game object with the specified name.
---@param name string The name for the new object
---@return GameObject
function Objects.create(name) end

---Deletes an object by its unique ID or Handle.
---@param id integer|GameObject The unique ID or handle of the object to delete
function Objects.deleteById(id) end

---Deletes all objects that have the specified name.
---@param name string The name of the objects to delete
function Objects.deleteByName(name) end

---Deletes all objects that have the specified tag.
---@param tag string The tag of the objects to delete
function Objects.deleteByTag(tag) end

---@class Camera
---@field transform Transform The transform component of the camera
---@field zoom number The zoom level of the camera (default is 1.0)
Camera = {}

---Global instance of the main Camera
---@type Camera
Camera = Camera or {}

---@class AudioSystem
---load sound
---@param name string the name to store music with it
---@param path string the path relative to engine binary
---@param isMusic boolean if its music so enter true and else enter false
function AudioSystem.load(name, path, isMusic) end