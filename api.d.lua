---@meta
-- JadidCore Lua API Documentation
-- Auto-complete & type hint only

--------------------------------------------------
-- Mouse Buttons
--------------------------------------------------

---@enum MouseButton
MouseButton = {
    Left   = "Left",
    Right  = "Right",
    Middle = "Middle"
}

--------------------------------------------------
-- Keyboard Keys (SDL Generated)
--------------------------------------------------

---@enum Key
Key = {
    A = "A",
    B = "B",
    C = "C",
    D = "D",
    E = "E",
    F = "F",
    G = "G",
    H = "H",
    I = "I",
    J = "J",
    K = "K",
    L = "L",
    M = "M",
    N = "N",
    O = "O",
    P = "P",
    Q = "Q",
    R = "R",
    S = "S",
    T = "T",
    U = "U",
    V = "V",
    W = "W",
    X = "X",
    Y = "Y",
    Z = "Z",
    _1 = "1",
    _2 = "2",
    _3 = "3",
    _4 = "4",
    _5 = "5",
    _6 = "6",
    _7 = "7",
    _8 = "8",
    _9 = "9",
    _0 = "0",
    Return = "Return",
    Escape = "Escape",
    Backspace = "Backspace",
    Tab = "Tab",
    Space = "Space",
    CapsLock = "CapsLock",
    F1 = "F1",
    F2 = "F2",
    F3 = "F3",
    F4 = "F4",
    F5 = "F5",
    F6 = "F6",
    F7 = "F7",
    F8 = "F8",
    F9 = "F9",
    F10 = "F10",
    F11 = "F11",
    F12 = "F12",
    PrintScreen = "PrintScreen",
    ScrollLock = "ScrollLock",
    Pause = "Pause",
    Insert = "Insert",
    Home = "Home",
    PageUp = "PageUp",
    Delete = "Delete",
    End = "End",
    PageDown = "PageDown",
    Right = "Right",
    Left = "Left",
    Down = "Down",
    Up = "Up",
    Numlock = "Numlock",
    Keypad_divide = "Keypad /",
    Keypad_star = "Keypad *",
    Keypad_minus = "Keypad -",
    Keypad_plus = "Keypad +",
    Keypad_Enter = "Keypad Enter",
    Keypad_1 = "Keypad 1",
    Keypad_2 = "Keypad 2",
    Keypad_3 = "Keypad 3",
    Keypad_4 = "Keypad 4",
    Keypad_5 = "Keypad 5",
    Keypad_6 = "Keypad 6",
    Keypad_7 = "Keypad 7",
    Keypad_8 = "Keypad 8",
    Keypad_9 = "Keypad 9",
    Keypad_0 = "Keypad 0",
    Keypad_dot = "Keypad .",
    NonUSBackslash = "NonUSBackslash",
    Application = "Application",
    Power = "Power",
    Keypad_equal = "Keypad =",
    F13 = "F13",
    F14 = "F14",
    F15 = "F15",
    F16 = "F16",
    F17 = "F17",
    F18 = "F18",
    F19 = "F19",
    F20 = "F20",
    F21 = "F21",
    F22 = "F22",
    F23 = "F23",
    F24 = "F24",
    Execute = "Execute",
    Help = "Help",
    Menu = "Menu",
    Select = "Select",
    Stop = "Stop",
    Again = "Again",
    Undo = "Undo",
    Cut = "Cut",
    Copy = "Copy",
    Paste = "Paste",
    Find = "Find",
    Mute = "Mute",
    VolumeUp = "VolumeUp",
    VolumeDown = "VolumeDown",
    Keypad_comma = "Keypad ,",
    International_1 = "International 1",
    International_2 = "International 2",
    International_3 = "International 3",
    International_4 = "International 4",
    International_5 = "International 5",
    International_6 = "International 6",
    International_7 = "International 7",
    International_8 = "International 8",
    International_9 = "International 9",
    Language_1 = "Language 1",
    Language_2 = "Language 2",
    Language_3 = "Language 3",
    Language_4 = "Language 4",
    Language_5 = "Language 5",
    Language_6 = "Language 6",
    Language_7 = "Language 7",
    Language_8 = "Language 8",
    Language_9 = "Language 9",
    AltErase = "AltErase",
    SysReq = "SysReq",
    Cancel = "Cancel",
    Clear = "Clear",
    Prior = "Prior",
    Separator = "Separator",
    Out = "Out",
    Oper = "Oper",
    Clear___Again = "Clear / Again",
    CrSel = "CrSel",
    ExSel = "ExSel",
    Keypad_00 = "Keypad 00",
    Keypad_000 = "Keypad 000",
    ThousandsSeparator = "ThousandsSeparator",
    DecimalSeparator = "DecimalSeparator",
    CurrencyUnit = "CurrencyUnit",
    CurrencySubUnit = "CurrencySubUnit",
    Keypad_Tab = "Keypad Tab",
    Keypad_Backspace = "Keypad Backspace",
    Keypad_A = "Keypad A",
    Keypad_B = "Keypad B",
    Keypad_C = "Keypad C",
    Keypad_D = "Keypad D",
    Keypad_E = "Keypad E",
    Keypad_F = "Keypad F",
    Keypad_XOR = "Keypad XOR",
    Keypad_MemStore = "Keypad MemStore",
    Keypad_MemRecall = "Keypad MemRecall",
    Keypad_MemClear = "Keypad MemClear",
    Keypad_MemAdd = "Keypad MemAdd",
    Keypad_MemSubtract = "Keypad MemSubtract",
    Keypad_MemMultiply = "Keypad MemMultiply",
    Keypad_MemDivide = "Keypad MemDivide",
    Keypad_Clear = "Keypad Clear",
    Keypad_ClearEntry = "Keypad ClearEntry",
    Keypad_Binary = "Keypad Binary",
    Keypad_Octal = "Keypad Octal",
    Keypad_Decimal = "Keypad Decimal",
    Keypad_Hexadecimal = "Keypad Hexadecimal",
    Left_Ctrl = "Left Ctrl",
    Left_Shift = "Left Shift",
    Left_Alt = "Left Alt",
    Left_GUI = "Left GUI",
    Right_Ctrl = "Right Ctrl",
    Right_Shift = "Right Shift",
    Right_Alt = "Right Alt",
    Right_GUI = "Right GUI",
    ModeSwitch = "ModeSwitch",
    AudioNext = "AudioNext",
    AudioPrev = "AudioPrev",
    AudioStop = "AudioStop",
    AudioPlay = "AudioPlay",
    AudioMute = "AudioMute",
    MediaSelect = "MediaSelect",
    WWW = "WWW",
    Mail = "Mail",
    Calculator = "Calculator",
    Computer = "Computer",
    AC_Search = "AC Search",
    AC_Home = "AC Home",
    AC_Back = "AC Back",
    AC_Forward = "AC Forward",
    AC_Stop = "AC Stop",
    AC_Refresh = "AC Refresh",
    AC_Bookmarks = "AC Bookmarks",
    BrightnessDown = "BrightnessDown",
    BrightnessUp = "BrightnessUp",
    DisplaySwitch = "DisplaySwitch",
    KBDIllumToggle = "KBDIllumToggle",
    KBDIllumDown = "KBDIllumDown",
    KBDIllumUp = "KBDIllumUp",
    Eject = "Eject",
    Sleep = "Sleep",
    App1 = "App1",
    App2 = "App2",
    AudioRewind = "AudioRewind",
    AudioFastForward = "AudioFastForward",
    SoftLeft = "SoftLeft",
    SoftRight = "SoftRight",
    Call = "Call",
    EndCall = "EndCall",
}

--------------------------------------------------
-- Global
--------------------------------------------------

---@type string
RUN_IN_SCENE = ""

--------------------------------------------------
-- Vector2
--------------------------------------------------

---@class vector2
---@field x number
---@field y number
vector2 = {}

---@return vector2
function vector2.new() end

--------------------------------------------------
-- GameObject
--------------------------------------------------

---@class GameObject
GameObject = {}

---@param name string
---@return string objectName
function GameObject.getObj(name) end

---@param name string
---@param delta vector2
function GameObject.movePos(name, delta) end

---@param name string
---@param pos vector2
function GameObject.setPos(name, pos) end

---@param name string
---@return vector2
function GameObject.getPos(name) end

---@param name string
---@param scale vector2
function GameObject.setScale(name, scale) end

--------------------------------------------------
-- Log
--------------------------------------------------

---@class Log
Log = {}

---@param text string
function Log.print(text) end

---@param text string
function Log.debug(text) end

---@param text string
function Log.info(text) end

---@param text string
function Log.warn(text) end

---@param text string
function Log.error(text) end

--- clear all logs
function Log.clear() end

--------------------------------------------------
-- Scene
--------------------------------------------------

---@class Scene
Scene = {}

---@param name string
function Scene.set(name) end

---@param code number
function Scene.exit(code) end

--------------------------------------------------
-- Input
--------------------------------------------------

---@class Input
Input = {}

---@param key Key
---@return boolean
function Input.keyPressed(key) end

---@param key Key
---@return boolean
function Input.keyDown(key) end

---@param key Key
---@return boolean
function Input.keyUp(key) end

---@param button MouseButton
---@return boolean
function Input.mousePressed(button) end

---@param button MouseButton
---@return boolean
function Input.mouseDown(button) end

---@param button MouseButton
---@return boolean
function Input.mouseUp(button) end

---@return vector2
function Input.mousePos() end

--------------------------------------------------
-- Lifecycle
--------------------------------------------------

function start() end

---@param dt number
function update(dt) end
