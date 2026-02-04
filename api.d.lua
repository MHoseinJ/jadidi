---@meta
-- JadidCore Engine API Declarations
-- This file is for IDE autocomplete & type checking only

--------------------------------------------------
-- Core Globals
--------------------------------------------------

---@type string
RUN_IN_SCENE = ""

--------------------------------------------------
-- Math
--------------------------------------------------

---@class Vector2
---@field x number
---@field y number
local Vector2 = {}

---@return Vector2
function Vector2.new() end

---@param x number
---@param y number
---@return Vector2
function Vector2.new(x, y) end

---@param x number
---@param y number
function Vector2:set(x, y) end

---@param dx number
---@param dy number
function Vector2:move(dx, dy) end

--------------------------------------------------
-- Transform
--------------------------------------------------

---@class Transform
---@field position Vector2
---@field scale Vector2
local Transform = {}

--------------------------------------------------
-- GameObject
--------------------------------------------------

---@class GameObject
---@field id number
---@field name string
---@field transform Transform
local GameObject = {}

--------------------------------------------------
-- Scene
--------------------------------------------------

---@class Scene
Scene = {}

---@param sceneFile string
function Scene.load(sceneFile) end

--------------------------------------------------
-- Objects (GameObject Manager)
--------------------------------------------------

---@class Objects
Objects = {}

---@overload fun(name: string): GameObject
---@overload fun(id: number): GameObject
---@return GameObject
function Objects.find(nameOrId) end

---@param name string
---@return GameObject
function Objects.create(name) end

---@param id number
function Objects.deleteById(id) end

---@param name string
function Objects.deleteByName(name) end

---@param tag string
function Objects.deleteByTag(tag) end

--------------------------------------------------
-- Input
--------------------------------------------------

---@class Input
Input = {}

---@overload fun(key: number): boolean
---@overload fun(key: string): boolean
---@return boolean
function Input.keyPressed(key) end

---@overload fun(key: number): boolean
---@overload fun(key: string): boolean
---@return boolean
function Input.keyDown(key) end

---@overload fun(key: number): boolean
---@overload fun(key: string): boolean
---@return boolean
function Input.keyUp(key) end

--------------------------------------------------
-- Mouse
--------------------------------------------------

---@class Mouse
Mouse = {}

---@return Vector2
function Mouse.position() end

--------------------------------------------------
-- Log / Debug
--------------------------------------------------

---@class Log
Log = {}

function Log.clear() end

---@param message any
function Log.print(message) end

---@param message any
function Log.info(message) end

---@param message any
function Log.warn(message) end

---@param message any
function Log.error(message) end

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

---@enum Mouse
Mouse = {
    LEFT = "1",
    RIGHT = "2",
    MIDDLE = "3",
    X1 = "4",
    X2 = "5",
}
