#include <stddef.h>
#include <stdint.h>
#include <io/Ports.hpp>
#include <kernel/IsrIrq.hpp>
#include <device/Keyboard.hpp>

using namespace angl;
using namespace device;

Keyboard Keyboard::_instance;
bool Keyboard::_hasInitialized = false;

void Keyboard::_handler(const kernel::RegisterSet &regs) {
    auto instance = Keyboard::instance();
    
    const auto scanCode = io::port::read(0x60);
    const bool released = scanCode & 0x80;
    const auto keyCode = static_cast<KeyCode>(
        released ? scanCode & ~0x80 : scanCode
    );
    
    if(keyCode == KeyCode::CapsLock) {
        if(!released) {
            instance->set(
                keyCode,
                (KeyState) { keyCode, !instance->getState(keyCode).pressed }
            );
            instance->refreshed = true;
        }
    } else if(instance->getState(keyCode).pressed != !released) {
        instance->set(keyCode, (KeyState) { keyCode, !released });
        instance->refreshed = true;
    }
}

Keyboard *Keyboard::instance() {
    if(!_hasInitialized) {
        auto interuptController = kernel::InterruptController::instance();
        interuptController->enableIrq(1);
        interuptController->handlers[33] = _handler;
        
        _hasInitialized = true;
    }
    return &_instance;
}

Keyboard::Keyboard() {
}

const KeyState &Keyboard::last() {
    return _keyState[static_cast<int>(_last)];
}

const KeyState &Keyboard::getState(const KeyCode &key) {
    return _keyState[static_cast<int>(key)];
}

void Keyboard::set(const KeyCode &key, const KeyState &state) {
    _keyState[static_cast<int>(key)] = state;
    _last = key;
}

// This is pretty long, but simple, so I just put it here at the end
char KeyState::toChar() const {
    auto keyboard = Keyboard::instance();
    
    if(keyboard->getState(KeyCode::LeftShift).pressed
            || keyboard->getState(KeyCode::RightShift).pressed
            || keyboard->getState(KeyCode::CapsLock).pressed) {
        switch(value) {
            case KeyCode::Escape:
            case KeyCode::F1:
            case KeyCode::F2:
            case KeyCode::F3:
            case KeyCode::F4:
            case KeyCode::F5:
            case KeyCode::F6:
            case KeyCode::F7:
            case KeyCode::F8:
            case KeyCode::F9:
            case KeyCode::F10:
            case KeyCode::F11:
            case KeyCode::F12:
            case KeyCode::ScrollLock:
            case KeyCode::NumLock:
            case KeyCode::Home:
            case KeyCode::PageUp:
            case KeyCode::PageDown:
            case KeyCode::End:
            case KeyCode::Insert:
            case KeyCode::Delete:
            case KeyCode::Backspace:
            case KeyCode::Alt:
            case KeyCode::Control:
            case KeyCode::LeftShift:
            case KeyCode::RightShift:
            case KeyCode::CapsLock:
            case KeyCode::UpArrow:
            case KeyCode::DownArrow:
            case KeyCode::LeftArrow:
            case KeyCode::RightArrow:
                return '\0';
            
            case KeyCode::Q:
                return 'Q';
            case KeyCode::W:
                return 'W';
            case KeyCode::E:
                return 'E';
            case KeyCode::R:
                return 'R';
            case KeyCode::T:
                return 'T';
            case KeyCode::Y:
                return 'Y';
            case KeyCode::U:
                return 'U';
            case KeyCode::I:
                return 'I';
            case KeyCode::O:
                return 'O';
            case KeyCode::P:
                return 'P';
            case KeyCode::A:
                return 'A';
            case KeyCode::S:
                return 'S';
            case KeyCode::D:
                return 'D';
            case KeyCode::F:
                return 'F';
            case KeyCode::G:
                return 'G';
            case KeyCode::H:
                return 'H';
            case KeyCode::J:
                return 'J';
            case KeyCode::K:
                return 'K';
            case KeyCode::L:
                return 'L';
            case KeyCode::Z:
                return 'Z';
            case KeyCode::X:
                return 'X';
            case KeyCode::C:
                return 'C';
            case KeyCode::V:
                return 'V';
            case KeyCode::B:
                return 'B';
            case KeyCode::N:
                return 'N';
            case KeyCode::M:
                return 'M';
            
            case KeyCode::Zero:
                return ')';
            case KeyCode::One:
                return '!';
            case KeyCode::Two:
                return '@';
            case KeyCode::Three:
                return '#';
            case KeyCode::Four:
                return '$';
            case KeyCode::Five:
                return '%';
            case KeyCode::Six:
                return '^';
            case KeyCode::Seven:
                return '&';
            case KeyCode::Eight:
                return '*';
            case KeyCode::Nine:
                return '(';
            
            case KeyCode::Minus:
                return '_';
            case KeyCode::Equals:
                return '+';
            case KeyCode::GraveAccent:
                return '~';
            case KeyCode::SemiColon:
                return ':';
            case KeyCode::Apostrophe:
                return '"';
            case KeyCode::LeftBracket:
                return '{';
            case KeyCode::RightBracket:
                return '}';
            case KeyCode::Comma:
                return '<';
            case KeyCode::Period:
                return '>';
            case KeyCode::BackSlash:
                return '|';
            case KeyCode::Slash:
                return '?';
            case KeyCode::Return:
                return '\n';
            case KeyCode::Tab:
                return '\t';
            case KeyCode::Space:
                return ' ';
        }
    } else {
        switch(value) {
            case KeyCode::Escape:
            case KeyCode::F1:
            case KeyCode::F2:
            case KeyCode::F3:
            case KeyCode::F4:
            case KeyCode::F5:
            case KeyCode::F6:
            case KeyCode::F7:
            case KeyCode::F8:
            case KeyCode::F9:
            case KeyCode::F10:
            case KeyCode::F11:
            case KeyCode::F12:
            case KeyCode::ScrollLock:
            case KeyCode::NumLock:
            case KeyCode::Home:
            case KeyCode::PageUp:
            case KeyCode::PageDown:
            case KeyCode::End:
            case KeyCode::Insert:
            case KeyCode::Delete:
            case KeyCode::Backspace:
            case KeyCode::Alt:
            case KeyCode::Control:
            case KeyCode::LeftShift:
            case KeyCode::RightShift:
            case KeyCode::CapsLock:
            case KeyCode::UpArrow:
            case KeyCode::DownArrow:
            case KeyCode::LeftArrow:
            case KeyCode::RightArrow:
                return '\0';
            
            case KeyCode::Q:
                return 'q';
            case KeyCode::W:
                return 'w';
            case KeyCode::E:
                return 'e';
            case KeyCode::R:
                return 'r';
            case KeyCode::T:
                return 't';
            case KeyCode::Y:
                return 'y';
            case KeyCode::U:
                return 'u';
            case KeyCode::I:
                return 'i';
            case KeyCode::O:
                return 'o';
            case KeyCode::P:
                return 'p';
            case KeyCode::A:
                return 'a';
            case KeyCode::S:
                return 's';
            case KeyCode::D:
                return 'd';
            case KeyCode::F:
                return 'f';
            case KeyCode::G:
                return 'g';
            case KeyCode::H:
                return 'h';
            case KeyCode::J:
                return 'j';
            case KeyCode::K:
                return 'k';
            case KeyCode::L:
                return 'l';
            case KeyCode::Z:
                return 'z';
            case KeyCode::X:
                return 'x';
            case KeyCode::C:
                return 'c';
            case KeyCode::V:
                return 'v';
            case KeyCode::B:
                return 'b';
            case KeyCode::N:
                return 'n';
            case KeyCode::M:
                return 'm';
            
            case KeyCode::Zero:
                return '0';
            case KeyCode::One:
                return '1';
            case KeyCode::Two:
                return '2';
            case KeyCode::Three:
                return '3';
            case KeyCode::Four:
                return '4';
            case KeyCode::Five:
                return '5';
            case KeyCode::Six:
                return '6';
            case KeyCode::Seven:
                return '7';
            case KeyCode::Eight:
                return '8';
            case KeyCode::Nine:
                return '9';
            
            case KeyCode::Minus:
                return '-';
            case KeyCode::Equals:
                return '=';
            case KeyCode::GraveAccent:
                return '`';
            case KeyCode::SemiColon:
                return ';';
            case KeyCode::Apostrophe:
                return '\'';
            case KeyCode::LeftBracket:
                return '[';
            case KeyCode::RightBracket:
                return ']';
            case KeyCode::Comma:
                return ',';
            case KeyCode::Period:
                return '.';
            case KeyCode::BackSlash:
                return '\\';
            case KeyCode::Slash:
                return '/';
            case KeyCode::Return:
                return '\n';
            case KeyCode::Tab:
                return '\t';
            case KeyCode::Space:
                return ' ';
        }
    }
    
    return '\0';
}
