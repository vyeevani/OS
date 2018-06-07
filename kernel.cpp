#include "types.h"
#include "gdt.h"
#include "interrupts.h"
#include "Keyboard.h"

void printf(char* str) {
    uint16_t* VideoMemory = (uint16_t*)0xb8000;

    static uint8_t x = 0, y = 0;
    for (int i = 0; str[i] != '\0'; ++i) {
        switch (str[i]) {
            case '\n':
                y++;
                x=0;
                break;
            default:
                VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
                x++;
        }


        if (x >= 80) {
            y++;
            x = 0;
        }

        if (y >= 25) {
            for (y = 0; y < 25; y++) {
                for (x = 0; x < 80; x++) {
                    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
                }
            }
            x = 0;
            y = 0;
        }
    }
}



/* params:
        pointer to multiboot_structure information about system i.e ram size
        magic number used to identify the operating system by the boot loader
*/
extern "C" void kernelMain(void* multiboot_structure, uint32_t magicNumber) {
    printf("Hello");
    printf("Hello");
    printf("Hello");
    printf("\nHello\n");
    printf("Hello");
    printf("Hello");

    GlobalDescriptorTable gdt;
    InterruptManager interrupts(&gdt);
    KeyboardDriver keyboard(&interrupts);
    interrupts.Activate();
    while(1);
}
