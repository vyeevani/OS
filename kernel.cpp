#include "types.h"
#include "gdt.h"

void printf(char* str) {
    uint16_t* VideoMemory = (uint16_t*)0xb8000;
    for (int i = 0; str[i] != '\0'; ++i) {
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
    }
}



/* params:
        pointer to multiboot_structure information about system i.e ram size
        magic number used to identify the operating system by the boot loader
*/
extern "C" void kernelMain(void* multiboot_structure, uint32_t magicNumber) {
    printf("Hello");

    GlobalDescriptorTable gdt;

    while(1);
}
