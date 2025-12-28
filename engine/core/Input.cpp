#include "Input.h"
#include <cstring>
#include <unordered_map>
#include "Log.h"

std::unordered_map<std::string, SDL_Scancode> Input::keyMap = {
    {"A", SDL_Scancode(4)},
    {"B", SDL_Scancode(5)},
    {"C", SDL_Scancode(6)},
    {"D", SDL_Scancode(7)},
    {"E", SDL_Scancode(8)},
    {"F", SDL_Scancode(9)},
    {"G", SDL_Scancode(10)},
    {"H", SDL_Scancode(11)},
    {"I", SDL_Scancode(12)},
    {"J", SDL_Scancode(13)},
    {"K", SDL_Scancode(14)},
    {"L", SDL_Scancode(15)},
    {"M", SDL_Scancode(16)},
    {"N", SDL_Scancode(17)},
    {"O", SDL_Scancode(18)},
    {"P", SDL_Scancode(19)},
    {"Q", SDL_Scancode(20)},
    {"R", SDL_Scancode(21)},
    {"S", SDL_Scancode(22)},
    {"T", SDL_Scancode(23)},
    {"U", SDL_Scancode(24)},
    {"V", SDL_Scancode(25)},
    {"W", SDL_Scancode(26)},
    {"X", SDL_Scancode(27)},
    {"Y", SDL_Scancode(28)},
    {"Z", SDL_Scancode(29)},
    {"1", SDL_Scancode(30)},
    {"2", SDL_Scancode(31)},
    {"3", SDL_Scancode(32)},
    {"4", SDL_Scancode(33)},
    {"5", SDL_Scancode(34)},
    {"6", SDL_Scancode(35)},
    {"7", SDL_Scancode(36)},
    {"8", SDL_Scancode(37)},
    {"9", SDL_Scancode(38)},
    {"0", SDL_Scancode(39)},
    {"Return", SDL_Scancode(40)},
    {"Escape", SDL_Scancode(41)},
    {"Backspace", SDL_Scancode(42)},
    {"Tab", SDL_Scancode(43)},
    {"Space", SDL_Scancode(44)},
    {"-", SDL_Scancode(45)},
    {"=", SDL_Scancode(46)},
    {"[", SDL_Scancode(47)},
    {"]", SDL_Scancode(48)},
    {"#", SDL_Scancode(50)},
    {";", SDL_Scancode(51)},
    {"'", SDL_Scancode(52)},
    {"`", SDL_Scancode(53)},
    {",", SDL_Scancode(54)},
    {".", SDL_Scancode(55)},
    {"/", SDL_Scancode(56)},
    {"CapsLock", SDL_Scancode(57)},
    {"F1", SDL_Scancode(58)},
    {"F2", SDL_Scancode(59)},
    {"F3", SDL_Scancode(60)},
    {"F4", SDL_Scancode(61)},
    {"F5", SDL_Scancode(62)},
    {"F6", SDL_Scancode(63)},
    {"F7", SDL_Scancode(64)},
    {"F8", SDL_Scancode(65)},
    {"F9", SDL_Scancode(66)},
    {"F10", SDL_Scancode(67)},
    {"F11", SDL_Scancode(68)},
    {"F12", SDL_Scancode(69)},
    {"PrintScreen", SDL_Scancode(70)},
    {"ScrollLock", SDL_Scancode(71)},
    {"Pause", SDL_Scancode(72)},
    {"Insert", SDL_Scancode(73)},
    {"Home", SDL_Scancode(74)},
    {"PageUp", SDL_Scancode(75)},
    {"Delete", SDL_Scancode(76)},
    {"End", SDL_Scancode(77)},
    {"PageDown", SDL_Scancode(78)},
    {"Right", SDL_Scancode(79)},
    {"Left", SDL_Scancode(80)},
    {"Down", SDL_Scancode(81)},
    {"Up", SDL_Scancode(82)},
    {"Numlock", SDL_Scancode(83)},
    {"Keypad /", SDL_Scancode(84)},
    {"Keypad *", SDL_Scancode(85)},
    {"Keypad -", SDL_Scancode(86)},
    {"Keypad +", SDL_Scancode(87)},
    {"Keypad Enter", SDL_Scancode(88)},
    {"Keypad 1", SDL_Scancode(89)},
    {"Keypad 2", SDL_Scancode(90)},
    {"Keypad 3", SDL_Scancode(91)},
    {"Keypad 4", SDL_Scancode(92)},
    {"Keypad 5", SDL_Scancode(93)},
    {"Keypad 6", SDL_Scancode(94)},
    {"Keypad 7", SDL_Scancode(95)},
    {"Keypad 8", SDL_Scancode(96)},
    {"Keypad 9", SDL_Scancode(97)},
    {"Keypad 0", SDL_Scancode(98)},
    {"Keypad .", SDL_Scancode(99)},
    {"NonUSBackslash", SDL_Scancode(100)},
    {"Application", SDL_Scancode(101)},
    {"Power", SDL_Scancode(102)},
    {"Keypad =", SDL_Scancode(103)},
    {"F13", SDL_Scancode(104)},
    {"F14", SDL_Scancode(105)},
    {"F15", SDL_Scancode(106)},
    {"F16", SDL_Scancode(107)},
    {"F17", SDL_Scancode(108)},
    {"F18", SDL_Scancode(109)},
    {"F19", SDL_Scancode(110)},
    {"F20", SDL_Scancode(111)},
    {"F21", SDL_Scancode(112)},
    {"F22", SDL_Scancode(113)},
    {"F23", SDL_Scancode(114)},
    {"F24", SDL_Scancode(115)},
    {"Execute", SDL_Scancode(116)},
    {"Help", SDL_Scancode(117)},
    {"Menu", SDL_Scancode(118)},
    {"Select", SDL_Scancode(119)},
    {"Stop", SDL_Scancode(120)},
    {"Again", SDL_Scancode(121)},
    {"Undo", SDL_Scancode(122)},
    {"Cut", SDL_Scancode(123)},
    {"Copy", SDL_Scancode(124)},
    {"Paste", SDL_Scancode(125)},
    {"Find", SDL_Scancode(126)},
    {"Mute", SDL_Scancode(127)},
    {"VolumeUp", SDL_Scancode(128)},
    {"VolumeDown", SDL_Scancode(129)},
    {"Keypad ,", SDL_Scancode(133)},
    {"Keypad = (AS400)", SDL_Scancode(134)},
    {"International 1", SDL_Scancode(135)},
    {"International 2", SDL_Scancode(136)},
    {"International 3", SDL_Scancode(137)},
    {"International 4", SDL_Scancode(138)},
    {"International 5", SDL_Scancode(139)},
    {"International 6", SDL_Scancode(140)},
    {"International 7", SDL_Scancode(141)},
    {"International 8", SDL_Scancode(142)},
    {"International 9", SDL_Scancode(143)},
    {"Language 1", SDL_Scancode(144)},
    {"Language 2", SDL_Scancode(145)},
    {"Language 3", SDL_Scancode(146)},
    {"Language 4", SDL_Scancode(147)},
    {"Language 5", SDL_Scancode(148)},
    {"Language 6", SDL_Scancode(149)},
    {"Language 7", SDL_Scancode(150)},
    {"Language 8", SDL_Scancode(151)},
    {"Language 9", SDL_Scancode(152)},
    {"AltErase", SDL_Scancode(153)},
    {"SysReq", SDL_Scancode(154)},
    {"Cancel", SDL_Scancode(155)},
    {"Clear", SDL_Scancode(156)},
    {"Prior", SDL_Scancode(157)},
    {"Return", SDL_Scancode(158)},
    {"Separator", SDL_Scancode(159)},
    {"Out", SDL_Scancode(160)},
    {"Oper", SDL_Scancode(161)},
    {"Clear / Again", SDL_Scancode(162)},
    {"CrSel", SDL_Scancode(163)},
    {"ExSel", SDL_Scancode(164)},
    {"Keypad 00", SDL_Scancode(176)},
    {"Keypad 000", SDL_Scancode(177)},
    {"ThousandsSeparator", SDL_Scancode(178)},
    {"DecimalSeparator", SDL_Scancode(179)},
    {"CurrencyUnit", SDL_Scancode(180)},
    {"CurrencySubUnit", SDL_Scancode(181)},
    {"Keypad (", SDL_Scancode(182)},
    {"Keypad )", SDL_Scancode(183)},
    {"Keypad {", SDL_Scancode(184)},
    {"Keypad }", SDL_Scancode(185)},
    {"Keypad Tab", SDL_Scancode(186)},
    {"Keypad Backspace", SDL_Scancode(187)},
    {"Keypad A", SDL_Scancode(188)},
    {"Keypad B", SDL_Scancode(189)},
    {"Keypad C", SDL_Scancode(190)},
    {"Keypad D", SDL_Scancode(191)},
    {"Keypad E", SDL_Scancode(192)},
    {"Keypad F", SDL_Scancode(193)},
    {"Keypad XOR", SDL_Scancode(194)},
    {"Keypad ^", SDL_Scancode(195)},
    {"Keypad %", SDL_Scancode(196)},
    {"Keypad <", SDL_Scancode(197)},
    {"Keypad >", SDL_Scancode(198)},
    {"Keypad &", SDL_Scancode(199)},
    {"Keypad &&", SDL_Scancode(200)},
    {"Keypad |", SDL_Scancode(201)},
    {"Keypad ||", SDL_Scancode(202)},
    {"Keypad :", SDL_Scancode(203)},
    {"Keypad #", SDL_Scancode(204)},
    {"Keypad Space", SDL_Scancode(205)},
    {"Keypad @", SDL_Scancode(206)},
    {"Keypad !", SDL_Scancode(207)},
    {"Keypad MemStore", SDL_Scancode(208)},
    {"Keypad MemRecall", SDL_Scancode(209)},
    {"Keypad MemClear", SDL_Scancode(210)},
    {"Keypad MemAdd", SDL_Scancode(211)},
    {"Keypad MemSubtract", SDL_Scancode(212)},
    {"Keypad MemMultiply", SDL_Scancode(213)},
    {"Keypad MemDivide", SDL_Scancode(214)},
    {"Keypad +/-", SDL_Scancode(215)},
    {"Keypad Clear", SDL_Scancode(216)},
    {"Keypad ClearEntry", SDL_Scancode(217)},
    {"Keypad Binary", SDL_Scancode(218)},
    {"Keypad Octal", SDL_Scancode(219)},
    {"Keypad Decimal", SDL_Scancode(220)},
    {"Keypad Hexadecimal", SDL_Scancode(221)},
    {"Left Ctrl", SDL_Scancode(224)},
    {"Left Shift", SDL_Scancode(225)},
    {"Left Alt", SDL_Scancode(226)},
    {"Left GUI", SDL_Scancode(227)},
    {"Right Ctrl", SDL_Scancode(228)},
    {"Right Shift", SDL_Scancode(229)},
    {"Right Alt", SDL_Scancode(230)},
    {"Right GUI", SDL_Scancode(231)},
    {"ModeSwitch", SDL_Scancode(257)},
    {"AudioNext", SDL_Scancode(258)},
    {"AudioPrev", SDL_Scancode(259)},
    {"AudioStop", SDL_Scancode(260)},
    {"AudioPlay", SDL_Scancode(261)},
    {"AudioMute", SDL_Scancode(262)},
    {"MediaSelect", SDL_Scancode(263)},
    {"WWW", SDL_Scancode(264)},
    {"Mail", SDL_Scancode(265)},
    {"Calculator", SDL_Scancode(266)},
    {"Computer", SDL_Scancode(267)},
    {"AC Search", SDL_Scancode(268)},
    {"AC Home", SDL_Scancode(269)},
    {"AC Back", SDL_Scancode(270)},
    {"AC Forward", SDL_Scancode(271)},
    {"AC Stop", SDL_Scancode(272)},
    {"AC Refresh", SDL_Scancode(273)},
    {"AC Bookmarks", SDL_Scancode(274)},
    {"BrightnessDown", SDL_Scancode(275)},
    {"BrightnessUp", SDL_Scancode(276)},
    {"DisplaySwitch", SDL_Scancode(277)},
    {"KBDIllumToggle", SDL_Scancode(278)},
    {"KBDIllumDown", SDL_Scancode(279)},
    {"KBDIllumUp", SDL_Scancode(280)},
    {"Eject", SDL_Scancode(281)},
    {"Sleep", SDL_Scancode(282)},
    {"App1", SDL_Scancode(283)},
    {"App2", SDL_Scancode(284)},
    {"AudioRewind", SDL_Scancode(285)},
    {"AudioFastForward", SDL_Scancode(286)},
    {"SoftLeft", SDL_Scancode(287)},
    {"SoftRight", SDL_Scancode(288)},
    {"Call", SDL_Scancode(289)},
    {"EndCall", SDL_Scancode(290)},
};

bool Input::quitRequested = false;

bool Input::currentKeys[SDL_NUM_SCANCODES] = { false };
bool Input::previousKeys[SDL_NUM_SCANCODES] = { false };

bool Input::currentMouse[8] = { false };
bool Input::previousMouse[8] = { false };

Vector2 Input::currentMousePosition = { 0, 0 };

std::unordered_map<std::string, int> Input::mouseMap = {
    {"Left",   SDL_BUTTON_LEFT},
    {"Right",  SDL_BUTTON_RIGHT},
    {"Middle", SDL_BUTTON_MIDDLE}
};

void Input::BeginFrame() {
    std::memcpy(previousKeys, currentKeys, sizeof(currentKeys));
    std::memcpy(previousMouse, currentMouse, sizeof(currentMouse));
}

void Input::Update() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            quitRequested = true;
            break;

        case SDL_KEYDOWN:
            if (event.key.repeat == 0)
                currentKeys[event.key.keysym.scancode] = true;
            break;

        case SDL_KEYUP:
            currentKeys[event.key.keysym.scancode] = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            currentMouse[event.button.button] = true;
            break;

        case SDL_MOUSEBUTTONUP:
            currentMouse[event.button.button] = false;
            break;

        case SDL_MOUSEMOTION:
            currentMousePosition.x = event.motion.x;
            currentMousePosition.y = event.motion.y;
            break;
        }
    }
}

bool Input::IsKeyPressed(SDL_Scancode code) {
    return currentKeys[code] && !previousKeys[code];
}

bool Input::IsKeyDown(SDL_Scancode code) {
    return currentKeys[code];
}

bool Input::IsKeyUp(SDL_Scancode code) {
    return !currentKeys[code] && previousKeys[code];
}

bool Input::IsMouseButtonPressed(int button) {
    return currentMouse[button] && !previousMouse[button];
}

bool Input::IsMouseButtonDown(int button) {
    return currentMouse[button];
}

bool Input::IsMouseButtonUp(int button) {
    return !currentMouse[button] && previousMouse[button];
}

bool Input::IsKeyPressed(const std::string& keyName) {
    SDL_Scancode code = static_cast<SDL_Scancode>(std::stoi(keyName));
    return IsKeyPressed(code);
}

bool Input::IsKeyDown(const std::string& keyName) {
    SDL_Scancode code = static_cast<SDL_Scancode>(std::stoi(keyName));
    return IsKeyDown(code);
}

bool Input::IsKeyUp(const std::string& keyName) {
    SDL_Scancode code = static_cast<SDL_Scancode>(std::stoi(keyName));
    return IsKeyUp(code);
}

bool Input::IsMouseButtonPressed(const std::string& buttonName) {
    auto it = mouseMap.find(buttonName);
    if (it == mouseMap.end()) return false;
    return IsMouseButtonPressed(it->second);
}

bool Input::IsMouseButtonDown(const std::string& buttonName) {
    auto it = mouseMap.find(buttonName);
    if (it == mouseMap.end()) return false;
    return IsMouseButtonDown(it->second);
}

bool Input::IsMouseButtonUp(const std::string& buttonName) {
    auto it = mouseMap.find(buttonName);
    if (it == mouseMap.end()) return false;
    return IsMouseButtonUp(it->second);
}

void Input::GetMousePosition(int& x, int& y) {
    x = currentMousePosition.x;
    y = currentMousePosition.y;
}

bool Input::QuitRequested() {
    return quitRequested;
}