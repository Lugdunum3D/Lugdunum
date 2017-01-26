#pragma once

#include <lug/Window/Export.hpp>
#ifdef LUG_SYSTEM_WINDOWS
#include <Windows.h>
#endif

namespace lug {
namespace Window {
namespace Keyboard {

#ifdef LUG_SYSTEM_WINDOWS

    enum class LUG_WINDOW_API Key : int {
        Unknown     = -1,               ///< Unhandled key
        A           = 'A',              ///< The A key
        B           = 'B',              ///< The B key
        C           = 'C',              ///< The C key
        D           = 'D',              ///< The D key
        E           = 'E',              ///< The E key
        F           = 'F',              ///< The F key
        G           = 'G',              ///< The G key
        H           = 'H',              ///< The H key
        I           = 'I',              ///< The I key
        J           = 'J',              ///< The J key
        K           = 'K',              ///< The K key
        L           = 'L',              ///< The L key
        M           = 'M',              ///< The M key
        N           = 'N',              ///< The N key
        O           = 'O',              ///< The O key
        P           = 'P',              ///< The P key
        Q           = 'Q',              ///< The Q key
        R           = 'R',              ///< The R key
        S           = 'S',              ///< The S key
        T           = 'T',              ///< The T key
        U           = 'U',              ///< The U key
        V           = 'V',              ///< The V key
        W           = 'W',              ///< The W key
        X           = 'X',              ///< The X key
        Y           = 'Y',              ///< The Y key
        Z           = 'Z',              ///< The Z key
        Num0        = '0',              ///< The 0 key
        Num1        = '1',              ///< The 1 key
        Num2        = '2',              ///< The 2 key
        Num3        = '3',              ///< The 3 key
        Num4        = '4',              ///< The 4 key
        Num5        = '5',              ///< The 5 key
        Num6        = '6',              ///< The 6 key
        Num7        = '7',              ///< The 7 key
        Num8        = '8',              ///< The 8 key
        Num9        = '9',              ///< The 9 key
        Escape      = VK_ESCAPE,        ///< The Escape key
        LControl    = VK_LCONTROL,      ///< The left Control key
        LShift      = VK_LSHIFT,        ///< The left Shift key
        LAlt        = VK_LMENU,         ///< The left Alt key
        LSystem     = VK_LWIN,          ///< The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
        RControl    = VK_RCONTROL,      ///< The right Control key
        RShift      = VK_RSHIFT,        ///< The right Shift key
        RAlt        = VK_RMENU,         ///< The right Alt key
        RSystem     = VK_RWIN,          ///< The right OS specific key: window (Windows and Linux), apple (MacOS X), ...
        Menu        = VK_APPS,          ///< The Menu key
        LBracket    = VK_OEM_4,         ///< The [ key
        RBracket    = VK_OEM_6,         ///< The ] key
        SemiColon   = VK_OEM_1,         ///< The , key
        Comma       = VK_OEM_COMMA,     ///< The , key
        Period      = VK_OEM_PERIOD,    ///< The . key
        Quote       = VK_OEM_7,         ///< The ' key
        Slash       = VK_OEM_2,         ///< The / key
        BackSlash   = VK_OEM_5,         ///< The \ key
        Tilde       = VK_OEM_3,         ///< The ~ key
        Equal       = VK_OEM_PLUS,      ///< The = key
        Dash        = VK_OEM_MINUS,     ///< The - key
        Space       = VK_SPACE,         ///< The Space key
        Return      = VK_RETURN,        ///< The Return key
        BackSpace   = VK_BACK,          ///< The Backspace key
        Tab         = VK_TAB,           ///< The Tabulation key
        PageUp      = VK_PRIOR,         ///< The Page up key
        PageDown    = VK_NEXT,          ///< The Page down key
        End         = VK_END,           ///< The End key
        Home        = VK_HOME,          ///< The Home key
        Insert      = VK_INSERT,        ///< The Insert key
        Delete      = VK_DELETE,        ///< The Delete key
        Add         = VK_ADD,           ///< The + key
        Subtract    = VK_SUBTRACT,      ///< The - key
        Multiply    = VK_MULTIPLY,      ///< The * key
        Divide      = VK_DIVIDE,        ///< The / key
        Left        = VK_LEFT,          ///< Left arrow
        Right       = VK_RIGHT,         ///< Right arrow
        Up          = VK_UP,            ///< Up arrow
        Down        = VK_DOWN,          ///< Down arrow
        Numpad0     = VK_NUMPAD0,       ///< The numpad 0 key
        Numpad1     = VK_NUMPAD1,       ///< The numpad 1 key
        Numpad2     = VK_NUMPAD2,       ///< The numpad 2 key
        Numpad3     = VK_NUMPAD3,       ///< The numpad 3 key
        Numpad4     = VK_NUMPAD4,       ///< The numpad 4 key
        Numpad5     = VK_NUMPAD5,       ///< The numpad 5 key
        Numpad6     = VK_NUMPAD6,       ///< The numpad 6 key
        Numpad7     = VK_NUMPAD7,       ///< The numpad 7 key
        Numpad8     = VK_NUMPAD8,       ///< The numpad 8 key
        Numpad9     = VK_NUMPAD9,       ///< The numpad 9 key
        F1          = VK_F1,            ///< The F1 key
        F2          = VK_F2,            ///< The F2 key
        F3          = VK_F3,            ///< The F3 key
        F4          = VK_F4,            ///< The F4 key
        F5          = VK_F5,            ///< The F5 key
        F6          = VK_F6,            ///< The F6 key
        F7          = VK_F7,            ///< The F7 key
        F8          = VK_F8,            ///< The F8 key
        F9          = VK_F9,            ///< The F9 key
        F10         = VK_F10,           ///< The F10 key
        F11         = VK_F11,           ///< The F11 key
        F12         = VK_F12,           ///< The F12 key
        F13         = VK_F13,           ///< The F13 key
        F14         = VK_F14,           ///< The F14 key
        F15         = VK_F15,           ///< The F15 key
        Pause       = VK_PAUSE,         ///< The Pause key
        KeyCount                        ///< Keep last -- the total number of keyboard keys
    };

#elif LUG_SYSTEM_ANDROID
#elif LUG_SYSTEM_LINUX
#endif

} // namespace Keyboard
} // namespace Window
} // namespace lug
