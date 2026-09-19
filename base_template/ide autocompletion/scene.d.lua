---@meta
-- Jadidi Engine API Declarations
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

---@param obj GameObject|nil
---@return boolean
function isValid(obj) end

------------------------------
----    SCENE  MANAGER    ----
------------------------------

---@class Scene
Scene = {}

---@param name string
function Scene.load(name) end

---@class Objects
Objects = {}

---@param name string
---@return GameObject|nil
function Objects.find(name) end

---@overload fun(id: integer): GameObject|nil
function Objects.find(id) end

---@param name string
---@return GameObject
function Objects.create(name) end

---@param id integer|GameObject
function Objects.deleteById(id) end

---@param name string
function Objects.deleteByName(name) end

---@param tag string
function Objects.deleteByTag(tag) end

---@class Camera
---@field transform Transform
---@field zoom number
Camera = {}

---@type Camera
Camera = Camera or {}

---@class AudioSystem
AudioSystem = {}

---@param name string
---@param path string
---@param isMusic boolean
function AudioSystem.load(name, path, isMusic) end