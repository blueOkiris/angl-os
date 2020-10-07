# Project settings
OSNAME :=  angl-os
AS :=      nasm
ASFLAGS := -f elf
CC :=      gcc
CFLAGS :=  -Iinclude -m32 -Wall -Werror -O2 -ffreestanding -fno-exceptions
LD :=      gcc
LDFLAGS := -nostdlib -nodefaultlibs -lgcc -m32

# Autogen stuff and helper stuff
LINKER :=  src/linker.ld
GRUB :=    src/grub.cfg
SRC :=     $(wildcard src/*.c) $(wildcard src/*.asm)
OBJS :=    $(subst .c,.c.o,$(subst .asm,.asm.o,$(subst src,obj,$(SRC))))
DISC :=    iso/$(OSNAME).iso

.PHONY : all
all : $(DISC)

obj/%.c.o : src/%.c
	mkdir -p obj/
	$(CC) $(CFLAGS) -c -o $@ $<

obj/%.asm.o : src/%.asm
	mkdir -p obj/
	$(AS) $(ASFLAGS) -o $@ $<

.PHONY : clean
clean :
	rm -rf obj/
	rm -rf iso/

# Main build project
iso/boot/$(OSNAME)-kernel : $(OBJS)
	rm -rf iso/
	mkdir -p iso
	mkdir -p iso/boot
	$(LD) $(OBJS) -T $(LINKER) -o iso/boot/$(OSNAME)-kernel $(LDFLAGS)

$(DISC) : iso/boot/$(OSNAME)-kernel $(GRUB)
	mkdir -p iso/boot/grub
	cp $(GRUB) iso/boot/grub
	grub-mkrescue iso --output=$(DISC)

.PHONY : run
run : $(DISC)
	qemu-system-x86_64 iso/angl-os.iso
