#include <terminal.h>

void kernel_main() {
    terminal.init();
    terminal.put_string("Welcome to angl-os!\n");
    terminal.put_string("Created by Dylan Turner\n");
    while(1);
}
