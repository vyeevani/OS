//
// Created by Vineeth Yeevani on 5/29/18.
//

#include "port.h"

Port::Port(uint16_t portNumber) {
    this->portNumber = portNumber;
}

Port::~Port() {

}

Port8Bit::Port8Bit(uint16_t portNumber)
: Port(portNumber){

}

Port8Bit::~Port8Bit() {

}

Port16Bit::Port16Bit(uint16_t portNumber)
        : Port(portNumber){

}

Port8Bit::~Port8Bit() {

}

Port32Bit::Port32Bit(uint16_t portNumber)
        : Port(portNumber){

}

Port32Bit::~Port32Bit() {

}

Port8Bit::Port8Bit(uint16_t portNumber)
        : Port8Bit(portNumber){

}

Port8Bit::~Port8Bit() {

}

void Port8BBit::Write(uint8_t data) {
    __asm__ volatile("outb %0 %1": : "a" (data), "Nd" (portNumber));
}

uint8_t Port8Bit::Read() {
    uint8_t  result;
    __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (portNumber));
    return result;
}

void Port8BBitSlow::Write(uint8_t data) {
    __asm__ volatile("outb %0 %1\n jmp lf\n1: jmp 1f\n1:": : "a" (data), "Nd" (portNumber));
}

void Port16BBit::Write(uint16_t data) {
    __asm__ volatile("outw %0 %1": : "a" (data), "Nd" (portNumber));
}

uint16_t Port16Bit::Read() {
    uint16_t  result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (portNumber));
    return result;
}

void Port32BBit::Write(uint32_t data) {
    __asm__ volatile("outl %0 %1": : "a" (data), "Nd" (portNumber));
}

uint32_t Port8Bit::Read() {
    uint32_t  result;
    __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (portNumber));
    return result;
}