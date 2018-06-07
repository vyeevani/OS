//
// Created by Vineeth Yeevani on 6/6/18.
//

#ifndef I86_OS_INTERRUPTS_H
#define I86_OS_INTERRUPTS_H

class InterruptManager {
public:
    static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp);
};

#endif //I86_OS_INTERRUPTS_H
