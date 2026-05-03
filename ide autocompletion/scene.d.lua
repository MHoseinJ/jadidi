---@meta
-- JadidCore Engine API Declarations
-- This file is for IDE autocomplete & type checking only (joke!)

------------------------------
----    SCENE  MANAGER    ----
------------------------------

---@class Scene
Scene = {}

---Loads a scene by its name.
---This function triggers the scene loading in the engine and loads the associated Lua scripts.
---@param name string The name of the scene to load
function Scene.load(name) end

---@class Objects
Objects = {}

---Finds a game object by its name or ID.
---Overloaded function: accepts either a string (name) or a number (id).
---@param name string The name of the object to find
---@return GameObject
function Objects.find(name) end

---@overload fun(id: integer): GameObject
function Objects.find(id) end

---Creates a new game object with the specified name.
---@param name string The name for the new object
---@return GameObject
function Objects.create(name) end

---Deletes an object by its unique ID.
---@param id integer The unique ID of the object to delete
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
---@param path string the path relative to engine binary for example we have a song in music folder in game folder and it should be: music/audio.wav
---@param isMusic boolean if its music so enter true and else enter false
function AudioSystem.load(name, path, isMusic) end
---@return void

AudioSystem = {}