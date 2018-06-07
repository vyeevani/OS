//
// Created by Vineeth Yeevani on 6/6/18.
//

#include "interrupts.h"

void printf(char* str);

static uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp) {
    printf(" Interrupt");
    return esp;
}

