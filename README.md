# ANGL Operating System

## Description

A small open source operating system

ANGL stands for "ANGL is Not GNU nor Linux"

## Operation Documentation

The first files to bring up are the header and implementation files for the IDT and GDT. They're under one location `IdtGdt.hpp` and `IdtGdt.cpp`. These descriptor tables are used for setting up memory locations for code and data segments and interrupt service routines. In this case, the GDT has been set up with four segments: Kernel Code, Kernel Data, User Code, User Data.

Then if you take a look in `Kernel.hpp`, you'll note that the Kernel has an IDT and a GDT. In the run function, these are initialized, filling their data and calling some assembly to load them into memory.

Now the IDT's data is a set of functions. They trigger on interrupts 0-31 and after talking to the interrupt controller through some ports, they trigger on interrupts 32 through 47 which are interrupt requests from devices. The core handlers are part of the `InterruptController` class which can be found in `IsrIrq.hpp`/`.cpp`. This `InterruptController` also has a set of handlers which can be overloaded to inject code when the interrupts trigger.

An example of this is the `Timer` class that represents the processor's real time clock. It's a singleton and when initialized, it can enable itself with `start`, disable itself with `stop`, and reinitialize itself with `setFrequency`. It adds a handler that increases a counter based on how the RTC has been configured.

The `Keyboard` works in a similar way. You get a pointer to it, and you check to see if there's been any input.

There's also a handler for a page fault exception which happens after you enable paging by calling the Kernel's `_enablePaging` function located in `Paging.cpp`. As it's part of the Kernel, there is no `Paging.hpp`. Currently it sets up three page tables, the first which basically just contains video memory, the kernel page which contains the `.text` linker section, and a filesystem page which will be used by the VFS.

Finally, another Singleton that exists is the `Terminal` class which allows one to interface with video memory. You can put a character, a string, or a (decimal) integer. It's files are `Terminal.hpp` and `Terminal.cpp`.

## Bootloader Options

I'd like to write my own bootloader, but after messing around, it's going to take more work, so right now, you can only use GRUB.

## Dependencies

If you want to use the GRUB bootloader, you'll need:

 1. `grub` - Not needed for your system to use (I use refind), but for `grub-mkrescue`

 2. `xorriso` - Needed for `grub-mkrescue` to work

For general compiling, you'll need:

 1. `gcc` - For c code

 2. `nasm` - For assembly

For emulation, you'll need:

 1. `qemu-system-x86_64` - For running
