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
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    _1,
    _2,
    _3,
    _4,
    _5,
    _6,
    _7,
    _8,
    _9,
    _0,
    Return,
    Escape,
    Backspace,
    Tab,
    Space,
    CapsLock,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    PrintScreen,
    ScrollLock,
    Pause,
    Insert,
    Home,
    PageUp,
    Delete,
    End,
    PageDown,
    Right,
    Left,
    Down,
    Up,
    Numlock,
    Keypad__,
    Keypad_star,
    Keypad__,
    Keypad_plus,
    Keypad_Enter,
    Keypad_1,
    Keypad_2,
    Keypad_3,
    Keypad_4,
    Keypad_5,
    Keypad_6,
    Keypad_7,
    Keypad_8,
    Keypad_9,
    Keypad_0,
    Keypad_dot,
    NonUSBackslash,
    Application,
    Power,
    Keypad_equal,
    F13,
    F14,
    F15,
    F16,
    F17,
    F18,
    F19,
    F20,
    F21,
    F22,
    F23,
    F24,
    Execute,
    Help,
    Menu,
    Select,
    Stop,
    Again,
    Undo,
    Cut,
    Copy,
    Paste,
    Find,
    Mute,
    VolumeUp,
    VolumeDown,
    Keypad_comma,
    International_1,
    International_2,
    International_3,
    International_4,
    International_5,
    International_6,
    International_7,
    International_8,
    International_9,
    Language_1,
    Language_2,
    Language_3,
    Language_4,
    Language_5,
    Language_6,
    Language_7,
    Language_8,
    Language_9,
    AltErase,
    SysReq,
    Cancel,
    Clear,
    Prior,
    Return,
    Separator,
    Out,
    Oper,
    Clear___Again,
    CrSel,
    ExSel,
    Keypad_00,
    Keypad_000,
    ThousandsSeparator,
    DecimalSeparator,
    CurrencyUnit,
    CurrencySubUnit,
    Keypad_Tab,
    Keypad_Backspace,
    Keypad_A,
    Keypad_B,
    Keypad_C,
    Keypad_D,
    Keypad_E,
    Keypad_F,
    Keypad_XOR,
    Keypad_MemStore,
    Keypad_MemRecall,
    Keypad_MemClear,
    Keypad_MemAdd,
    Keypad_MemSubtract,
    Keypad_MemMultiply,
    Keypad_MemDivide,
    Keypad_Clear,
    Keypad_ClearEntry,
    Keypad_Binary,
    Keypad_Octal,
    Keypad_Decimal,
    Keypad_Hexadecimal,
    Left_Ctrl,
    Left_Shift,
    Left_Alt,
    Left_GUI,
    Right_Ctrl,
    Right_Shift,
    Right_Alt,
    Right_GUI,
    ModeSwitch,
    AudioNext,
    AudioPrev,
    AudioStop,
    AudioPlay,
    AudioMute,
    MediaSelect,
    WWW,
    Mail,
    Calculator,
    Computer,
    AC_Search,
    AC_Home,
    AC_Back,
    AC_Forward,
    AC_Stop,
    AC_Refresh,
    AC_Bookmarks,
    BrightnessDown,
    BrightnessUp,
    DisplaySwitch,
    KBDIllumToggle,
    KBDIllumDown,
    KBDIllumUp,
    Eject,
    Sleep,
    App1,
    App2,
    AudioRewind,
    AudioFastForward,
    SoftLeft,
    SoftRight,
    Call,
    EndCall,
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
