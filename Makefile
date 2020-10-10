# Project settings
OSNAME :=   angl-os
DISCFLDR := disc
AS :=       nasm
ASFLAGS :=  -f elf32
CPPC :=     g++
CPPFLAGS := -Iinclude -m32 -O2 -Wall -Werror \
            -fno-rtti -ffreestanding -fno-exceptions
LD :=       ld
LDFLAGS :=  -static -m elf_i386

# Autogen stuff and helper stuff
LINKER :=   src/linker.ld
GRUB :=     src/grub.cfg
SRC :=      $(wildcard src/*.asm) $(wildcard src/*.cpp) \
            $(wildcard src/device/*.asm) $(wildcard src/device/*.cpp) \
            $(wildcard src/io/*.asm) $(wildcard src/io/*.cpp) \
            $(wildcard src/kernel/*.asm) $(wildcard src/kernel/*.cpp)
OBJS :=     $(subst .cpp,.cpp.o,$(subst .asm,.asm.o,$(subst src,obj,$(SRC))))
DISC :=     $(DISCFLDR)/$(OSNAME).img

.PHONY : all
all : $(DISC)

obj/%.cpp.o : src/%.cpp
	mkdir -p obj/
	mkdir -p obj/io
	mkdir -p obj/device
	mkdir -p obj/kernel
	$(CPPC) $(CPPFLAGS) -c -o $@ $<

obj/%.asm.o : src/%.asm
	mkdir -p obj/
	$(AS) $(ASFLAGS) -o $@ $<

.PHONY : clean
clean :
	rm -rf obj/
	rm -rf $(DISCFLDR)/

# Main build project
$(DISCFLDR)/boot/$(OSNAME)-kernel : $(OBJS) $(LINKER)
	rm -rf $(DISCFLDR)/
	mkdir -p $(DISCFLDR)
	mkdir -p $(DISCFLDR)/boot
	$(LD) $(OBJS) -T $(LINKER) -o $(DISCFLDR)/boot/$(OSNAME)-kernel $(LDFLAGS)

$(DISC) : $(DISCFLDR)/boot/$(OSNAME)-kernel $(GRUB)
	mkdir -p $(DISCFLDR)/boot/grub
	cp $(GRUB) $(DISCFLDR)/boot/grub
	grub-mkrescue $(DISCFLDR) --output=$(DISC)

.PHONY : run
run : $(DISC)
	qemu-system-x86_64 $(DISC)
