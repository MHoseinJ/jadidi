---@meta
-- JadidCore Engine API Declarations
-- This file is for IDE autocomplete & type checking only (joke!)

------------------------------
----         CORE         ----
------------------------------

---The name of the scene this script belongs to.
---This variable is required for the script to be loaded in the specific scene.
---@type string
RUN_IN_SCENE = ""

---Called once when the object is created or the scene starts.
---This function is optional.
function start() end

---Called every frame.
---This function is optional.
---@param dt number The delta time in seconds
function update(dt) end

------------------------------
----         MATH         ----
------------------------------

---@class Vector2
--- x axis field
---@field x number
--- y axis field
---@field y number
local Vector2 = {}

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

------------------------------
----    SCENE  MANAGER    ----
------------------------------

---@class Scene
local Scene = {}

---Loads a scene by its name.
---This function triggers the scene loading in the engine and loads the associated Lua scripts.
---@param name string The name of the scene to load
function Scene.load(name) end


---@class Objects
local Objects = {}

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
local Camera = {}

---Global instance of the main Camera
---@type Camera
Camera = Camera or {}

------------------------------
----   COMPONENT SYSTEM   ----
------------------------------

---@class Component
---The GameObject that owns this component
---@field owner GameObject
local Component = {}

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
local Transform = {}


---@class Sprite : Component
---The file path to the texture asset
---@field path string
---The rendering order (higher values draw on top)
---@field z_index integer
local Sprite = {}

---Reloads the sprite texture.
---This releases the current texture and loads it again based on the path.
function Sprite:reload() end


---@class Animator : Component
local Animator = {}

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
local Rigidbody = {}


---@class GameObject
---The unique ID of the game object
---@field id integer
---The name of the game object
---@field name string
---The Transform component attached to this object
---@field transform Transform
local GameObject = {}

---Adds a component of the specified type to this game object.
---@param name string The type name of the component (e.g., "Sprite", "Animator")
---@return Component The newly created component
function GameObject:addComponent(name) end

---Gets a component of the specified type attached to this game object.
---@param name string The type name of the component (e.g., "Sprite", "Animator")
---@return Component The component if found, otherwise nil
function GameObject:getComponent(name) end

------------------------------
----     STATE SYSTEM     ----
------------------------------

---@class State
local State = {}

---Sets a value in the global state with the given key.
---@param key string The key to identify the value
---@param value any The value to store (can be number, string, table, etc.)
function State.set(key, value) end

---Gets a value from the global state by key.
---@param key string The key to retrieve
---@return any|nil The stored value, or nil if the key does not exist
function State.get(key) end

---Checks if a key exists in the global state.
---@param key string The key to check
---@return boolean True if the key exists, false otherwise
function State.exists(key) end

---Removes a key-value pair from the global state.
---@param key string The key to remove
function State.remove(key) end

---Clears all data from the global state.
function State.clear() end

------------------------------
----         JSON         ----
------------------------------

---@class Json
local Json = {}

---Reads a JSON file and converts it to a Lua table.
---@param path string The file path to the JSON file
---@return table|nil The parsed data as a Lua table, or nil if the file does not exist
function Json.read(path) end

---Writes a Lua table to a JSON file.
---@param path string The file path where the JSON should be saved
---@param table table The Lua table data to convert and write
---@return boolean True if the write operation was successful, false otherwise
function Json.write(path, table) end

------------------------------
----        INPUT         ----
------------------------------

---@class Input
local Input = {}

---Checks if a key was just pressed (triggered once per press).
---@param scancode integer The scancode of the key
---@return boolean True if the key was just pressed
function Input.keyPressed(scancode) end

---Checks if a key is currently held down.
---@param scancode integer The scancode of the key
---@return boolean True if the key is down
function Input.keyDown(scancode) end

---Checks if a key was just released.
---@param scancode integer The scancode of the key
---@return boolean True if the key was just released
function Input.keyUp(scancode) end

---Checks if a mouse button was just pressed (triggered once per click).
---@param button integer The button code (e.g., 1 for left, 2 for middle, 3 for right)
---@return boolean True if the button was just pressed
function Input.mousePressed(button) end

---Checks if a mouse button is currently held down.
---@param button integer The button code
---@return boolean True if the button is down
function Input.mouseDown(button) end

---Checks if a mouse button was just released.
---@param button integer The button code
---@return boolean True if the button was just released
function Input.mouseUp(button) end


---@class Mouse
local Mouse = {}

---Gets the current position of the mouse cursor.
---@return Vector2 The x and y coordinates of the mouse
function Mouse.position() end

---Gets the current position of the mouse cursor.
---@return Vector2 The x and y coordinates of the mouse
function Mouse.position() end

---Mouse button constant for the Left button.
---@type string
Mouse.LEFT = "1"

---Mouse button constant for the Middle button (scroll wheel click).
---@type string
Mouse.MIDDLE = "2"

---Mouse button constant for the Right button.
---@type string
Mouse.RIGHT = "3"

---Mouse button constant for the X1 (side) button.
---@type string
Mouse.X1 = "4"

---Mouse button constant for the X2 (side) button.
---@type string
Mouse.X2 = "5"

------------------------------
----        LOGGER        ----
------------------------------

---@class Log
local Log = {}

---Clears the log output.
function Log.clear() end

---Prints a general message to the log.
---@param str string The message to print
function Log.print(str) end

---Logs an informational message.
---@param str string The message to log
function Log.info(str) end

---Logs a warning message.
---@param str string The warning message to log
function Log.warn(str) end

---Logs an error message.
---@param str string The error message to log
function Log.error(str) end

------------------------------
----         KEYS         ----
------------------------------

---@enum Key
Key = {
    A = "4",
    B = "5",
    C = "6",
    D = "7",
    E = "8",
    F = "9",
    G = "10",
    H = "11",
    I = "12",
    J = "13",
    K = "14",
    L = "15",
    M = "16",
    N = "17",
    O = "18",
    P = "19",
    Q = "20",
    R = "21",
    S = "22",
    T = "23",
    U = "24",
    V = "25",
    W = "26",
    X = "27",
    Y = "28",
    Z = "29",
    _1 = "30",
    _2 = "31",
    _3 = "32",
    _4 = "33",
    _5 = "34",
    _6 = "35",
    _7 = "36",
    _8 = "37",
    _9 = "38",
    _0 = "39",
    RETURN = "40",
    ESCAPE = "41",
    BACKSPACE = "42",
    TAB = "43",
    SPACE = "44",
    _ = "45",
    _ = "46",
    _ = "47",
    _ = "48",
    _ = "49",
    _ = "50",
    _ = "51",
    _ = "52",
    _ = "53",
    _ = "54",
    _ = "55",
    _ = "56",
    CAPSLOCK = "57",
    F1 = "58",
    F2 = "59",
    F3 = "60",
    F4 = "61",
    F5 = "62",
    F6 = "63",
    F7 = "64",
    F8 = "65",
    F9 = "66",
    F10 = "67",
    F11 = "68",
    F12 = "69",
    PRINTSCREEN = "70",
    SCROLLLOCK = "71",
    PAUSE = "72",
    INSERT = "73",
    HOME = "74",
    PAGEUP = "75",
    DELETE = "76",
    END = "77",
    PAGEDOWN = "78",
    RIGHT = "79",
    LEFT = "80",
    DOWN = "81",
    UP = "82",
    NUMLOCK = "83",
    KEYPAD = "84",
    KEYPAD = "85",
    KEYPAD = "86",
    KEYPAD = "87",
    KEYPAD_ENTER = "88",
    KEYPAD_1 = "89",
    KEYPAD_2 = "90",
    KEYPAD_3 = "91",
    KEYPAD_4 = "92",
    KEYPAD_5 = "93",
    KEYPAD_6 = "94",
    KEYPAD_7 = "95",
    KEYPAD_8 = "96",
    KEYPAD_9 = "97",
    KEYPAD_0 = "98",
    KEYPAD = "99",
    NONUSBACKSLASH = "100",
    APPLICATION = "101",
    POWER = "102",
    KEYPAD = "103",
    F13 = "104",
    F14 = "105",
    F15 = "106",
    F16 = "107",
    F17 = "108",
    F18 = "109",
    F19 = "110",
    F20 = "111",
    F21 = "112",
    F22 = "113",
    F23 = "114",
    F24 = "115",
    EXECUTE = "116",
    HELP = "117",
    MENU = "118",
    SELECT = "119",
    STOP = "120",
    AGAIN = "121",
    UNDO = "122",
    CUT = "123",
    COPY = "124",
    PASTE = "125",
    FIND = "126",
    MUTE = "127",
    VOLUMEUP = "128",
    VOLUMEDOWN = "129",
    KEYPAD = "133",
    KEYPAD_AS400 = "134",
    INTERNATIONAL_1 = "135",
    INTERNATIONAL_2 = "136",
    INTERNATIONAL_3 = "137",
    INTERNATIONAL_4 = "138",
    INTERNATIONAL_5 = "139",
    INTERNATIONAL_6 = "140",
    INTERNATIONAL_7 = "141",
    INTERNATIONAL_8 = "142",
    INTERNATIONAL_9 = "143",
    LANGUAGE_1 = "144",
    LANGUAGE_2 = "145",
    LANGUAGE_3 = "146",
    LANGUAGE_4 = "147",
    LANGUAGE_5 = "148",
    LANGUAGE_6 = "149",
    LANGUAGE_7 = "150",
    LANGUAGE_8 = "151",
    LANGUAGE_9 = "152",
    ALTERASE = "153",
    SYSREQ = "154",
    CANCEL = "155",
    CLEAR = "156",
    PRIOR = "157",
    RETURN = "158",
    SEPARATOR = "159",
    OUT = "160",
    OPER = "161",
    CLEAR_AGAIN = "162",
    CRSEL = "163",
    EXSEL = "164",
    KEYPAD_00 = "176",
    KEYPAD_000 = "177",
    THOUSANDSSEPARATOR = "178",
    DECIMALSEPARATOR = "179",
    CURRENCYUNIT = "180",
    CURRENCYSUBUNIT = "181",
    KEYPAD = "182",
    KEYPAD = "183",
    KEYPAD = "184",
    KEYPAD = "185",
    KEYPAD_TAB = "186",
    KEYPAD_BACKSPACE = "187",
    KEYPAD_A = "188",
    KEYPAD_B = "189",
    KEYPAD_C = "190",
    KEYPAD_D = "191",
    KEYPAD_E = "192",
    KEYPAD_F = "193",
    KEYPAD_XOR = "194",
    KEYPAD = "195",
    KEYPAD = "196",
    KEYPAD = "197",
    KEYPAD = "198",
    KEYPAD = "199",
    KEYPAD = "200",
    KEYPAD = "201",
    KEYPAD = "202",
    KEYPAD = "203",
    KEYPAD = "204",
    KEYPAD_SPACE = "205",
    KEYPAD = "206",
    KEYPAD = "207",
    KEYPAD_MEMSTORE = "208",
    KEYPAD_MEMRECALL = "209",
    KEYPAD_MEMCLEAR = "210",
    KEYPAD_MEMADD = "211",
    KEYPAD_MEMSUBTRACT = "212",
    KEYPAD_MEMMULTIPLY = "213",
    KEYPAD_MEMDIVIDE = "214",
    KEYPAD = "215",
    KEYPAD_CLEAR = "216",
    KEYPAD_CLEARENTRY = "217",
    KEYPAD_BINARY = "218",
    KEYPAD_OCTAL = "219",
    KEYPAD_DECIMAL = "220",
    KEYPAD_HEXADECIMAL = "221",
    LEFT_CTRL = "224",
    LEFT_SHIFT = "225",
    LEFT_ALT = "226",
    LEFT_GUI = "227",
    RIGHT_CTRL = "228",
    RIGHT_SHIFT = "229",
    RIGHT_ALT = "230",
    RIGHT_GUI = "231",
    MODESWITCH = "257",
    AUDIONEXT = "258",
    AUDIOPREV = "259",
    AUDIOSTOP = "260",
    AUDIOPLAY = "261",
    AUDIOMUTE = "262",
    MEDIASELECT = "263",
    WWW = "264",
    MAIL = "265",
    CALCULATOR = "266",
    COMPUTER = "267",
    AC_SEARCH = "268",
    AC_HOME = "269",
    AC_BACK = "270",
    AC_FORWARD = "271",
    AC_STOP = "272",
    AC_REFRESH = "273",
    AC_BOOKMARKS = "274",
    BRIGHTNESSDOWN = "275",
    BRIGHTNESSUP = "276",
    DISPLAYSWITCH = "277",
    KBDILLUMTOGGLE = "278",
    KBDILLUMDOWN = "279",
    KBDILLUMUP = "280",
    EJECT = "281",
    SLEEP = "282",
    APP1 = "283",
    APP2 = "284",
    AUDIOREWIND = "285",
    AUDIOFASTFORWARD = "286",
    SOFTLEFT = "287",
    SOFTRIGHT = "288",
    CALL = "289",
    ENDCALL = "290",
}