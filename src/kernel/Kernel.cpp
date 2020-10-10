#include <device/Timer.hpp>
#include <device/Keyboard.hpp>
#include <io/Terminal.hpp>
#include <kernel/IdtGdt.hpp>
#include <kernel/Kernel.hpp>

using namespace angl;
using namespace kernel;

void Kernel::run() {
    _gdt.init();
    _idt.init();
    _enablePaging();
    _fs.init();
    
    _terminal = io::Terminal::instance();
    _terminal->putStr("Welcome to ANGL OS!\nCreated by Dylan Turner\n");

    _testIdt();
    _testIrqThroughTimer();
    _testKeyboard();
    _testFileSystem();
    //_testPageFault();

    while(true);
}

void Kernel::_testIdt() {
    _terminal->putStr("\nTesting IDT...\n");
    asm volatile ("int $0");
    asm volatile ("int $31");
    _terminal->putStr("Done testing.\n");
}

void Kernel::_testIrqThroughTimer() {
    _terminal->putStr("\nTesting IRQs...\n");
    _terminal->putStr("  RTC test:\n");
    
    auto timer = device::Timer::instance();
    timer->setFrequency(50);
    timer->start();
    uint32_t lastTicks = timer->ticks();
    while(lastTicks < 10) {
        if(lastTicks != timer->ticks()) {
            lastTicks = timer->ticks();
            _terminal->putStr("    Ticks: ");
            _terminal->putInteger(timer->ticks());
            _terminal->putChar('\n');
        }
    }
    timer->stop();
    
    _terminal->putStr("  Done.\n");
    _terminal->putStr("Done testing.\n");
}

void Kernel::_testKeyboard() {
    _terminal->putStr("\nTesting keyboard...\n");
    _terminal->putStr("Press Escape to quit!\n");
    
    auto keyboard = device::Keyboard::instance();
    while(!keyboard->getState(device::KeyCode::Escape).pressed) {
        if(keyboard->refreshed) {
            if(keyboard->last().pressed 
                    && keyboard->last().value != device::KeyCode::LeftShift
                    && keyboard->last().value != device::KeyCode::RightShift) {
                _terminal->putChar(keyboard->last().toChar());
            }
            keyboard->refreshed = false;
        }
    }
    
    _terminal->putStr("\nDone testing.\n");
}

void Kernel::_testFileSystem() {
    _terminal->putStr("Testing file system...\n");
    
    _terminal->putStr("  Root file system:\n");
    auto i = 0;
    io::DirEntry dirEntry = { NULL, NULL, NULL };
    do {
        dirEntry = _fs.readDirEntryByIndex(i++);
        if(dirEntry.fileName != NULL) {
            _terminal->putStr("  ");
            _terminal->putStr(dirEntry.fileName);
            _terminal->putStr("    ");
            _terminal->putInteger(*(dirEntry.fileType));
            _terminal->putChar('\n');
        }
    } while(dirEntry.fileName != NULL);
    _terminal->putStr("Done testing.\n");
}

void Kernel::_testPageFault() {
    _terminal->putStr("Forcing a page fault...\n");
    uint32_t *ptr = (uint32_t *) 0xA0000000;
    uint32_t doPageFault = *ptr;
    _terminal->putInteger(doPageFault);
}
