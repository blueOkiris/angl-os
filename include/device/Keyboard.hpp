#pragma once

#include <stddef.h>
#include <stdint.h>
#include <kernel/IsrIrq.hpp>

namespace angl {
    namespace device {
        enum class KeyCode {
            Escape = 1,
            F1 = 59, F2 = 60, F3 = 61, F4 = 62, F5 = 63, F6 = 64,
            F7 = 65, F8 = 66, F9 = 67, F10 = 68, F11 = 96, F12 = 97,
            ScrollLock = 70, NumLock = 69, Home = 71,
            PageUp = 73, PageDown = 90, End = 79,
            Insert = 91, Delete = 92,
            
            Zero = 11, One = 2, Two = 3, Three = 4, Four = 5,
            Five = 6, Six = 7, Seven = 8, Eight = 9, Nine = 10, 
            
            Q = 16, W = 17, E = 18, R = 19, T = 20, Y = 21,
            U = 22, I = 23, O = 24, P = 25,
            A = 30, S = 31, D = 32, F = 33, G = 34,
            H = 35, J = 36, K = 37, L = 38,
            Z = 44, X = 45, C = 46, V = 47, B = 48, N = 49, M = 50,
            
            Minus = 12, Equals = 13, GraveAccent = 41,
            SemiColon = 39, Apostrophe = 40,
            LeftBracket = 26, RightBracket = 27,
            Comma = 51, Period = 52,
            BackSlash = 43, Slash = 53,
            
            Return = 28, Tab = 15, Backspace = 14, Space = 57,
            
            Alt = 56, Control = 29, LeftShift = 42, RightShift = 54,
            CapsLock = 58,
            
            UpArrow = 72, DownArrow = 80, LeftArrow = 75, RightArrow = 77
        };
        
        struct KeyState {
            KeyCode value;
            bool pressed;
            
            char toChar() const;
        };
        
        class Keyboard {
            private:
                static Keyboard _instance;
                static bool _hasInitialized;
                
                KeyState _keyState[98];
                KeyCode _last;
                
                static void _handler(const kernel::RegisterSet &regs);
                Keyboard();
            
            public:
                bool refreshed;
                
                static Keyboard *instance();
                const KeyState &getState(const KeyCode &key);
                const KeyState &last();
                void set(const KeyCode &key, const KeyState &state);
        };
    }
}