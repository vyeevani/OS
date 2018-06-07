//
// Created by Vineeth Yeevani on 6/6/18.
//

#include "interrupts.h"

void printf(char* str);

InterruptManager* InterruptManager::ActiveInterruptManager = 0;


InterruptManager::InterruptManager(GlobalDescriptorTable* gdt) : picMasterCommand(0x20),
								 picMasterData(0x21),
								 picSlaveCommand(0xA0),
								 picSlaveData(0xA1){
  uint16_t CodeSegment = gdt->CodeSegmentSelector();
  const uint8_t IDT_INTERRUPT_GATE = 0xE;
  for (uint16_t i = 0; i < 256; i++) {
    SetInterruptDescriptorTableEntry(i, CodeSegment, &InterruptIgnore, 0, IDT_INTERRUPT_GATE);
  }
  SetInterruptDescriptorTableEntry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
  SetInterruptDescriptorTableEntry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);

  picMasterCommand.Write(0x11);
  picSlaveCommand.Write(0x11);

  //Assign pic offsets to prevent CPU reset
  picMasterData.Write(0x20);
  picSlaveData.Write(0x28);

  //Assign pic to master/slave
  picMasterData.Write(0x04);
  picSlaveData.Write(0x02);

  picMasterData.Write(0x01);
  picSlaveData.Write(0x01);

  picMasterData.Write(0x00);
  picSlaveData.Write(0x00);

  
  InterruptDescriptorTablePointer idt;
  idt.size = 256 * sizeof(GateDescriptor) - 1;
  idt.base = (uint32_t)interruptDescriptorTable;
  asm volatile("lidt %0": :"m" (idt)); 
} 

InterruptManager::~InterruptManager() {
}

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

void InterruptManager::SetInterruptDescriptorTableEntry(uint8_t interruptNumber,
							uint16_t codeSegmentSelectorOffset,
							void (*handler)(),
							uint8_t DescriptorPriviledgeLevel,
							uint8_t DescriptorType
							) {
  const uint8_t IDT_DESC_PRESENT = 0X80;
  
  interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t) handler) & 0xFFFF;
  interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset;
  interruptDescriptorTable[interruptNumber].reserved = 0;
  interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | DescriptorType | ((DescriptorPriviledgeLevel&3) << 5);
  interruptDescriptorTable[interruptNumber].handlerAddressHighBits = (((uint32_t) handler) >> 16) & 0xFFF;
}



uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp) {
  if (ActiveInterruptManager != 0){
    return ActiveInterruptManager->DoHandleInterrupt(interruptNumber, esp);
  } else {
    return esp;
  }
}

uint32_t InterruptManager::DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp) {
  printf(" Interrupt");
  return esp;
}

void InterruptManager::Activate() {
  if (ActiveInterruptManager != 0) {
    ActiveInterruptManager->Deactivate();
  }
  ActiveInterruptManager = this;
  asm("sti");
}

void InterruptManager::Deactivate() {
  if (ActiveInterruptManager == this) {
    ActiveInterruptManager = 0;
    asm("cli");
  }
}
