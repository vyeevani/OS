#include "Mouse.h"

MouseDriver::MouseDriver(InterruptManager* interruptManager) : InterruptHandler(0x2C, interruptManager), dataPort(0x60), commandPort(0x64) {
  offset = 0;
  buttons = 0;

  uint16_t* VideoMemory = (uint16_t*)0xb8000;
  
    VideoMemory[80 * 12 + 40] = ((VideoMemory[80 * 12 + 40] & 0xF000) >> 4)
      | ((VideoMemory[80 * 12 + 40] & 0x0F00) << 4)
      | (VideoMemory[80 * 12 + 40] & 0x00FF);
  commandPort.Write(0xA8); // Activate interrupts
  commandPort.Write(0x20); // Get current state
  uint8_t status = (dataPort.Read() | 2);
  commandPort.Write(0x60); // Set state
  dataPort.Write(status);

  commandPort.Write(0xD4);
  dataPort.Write(0xF4);    // Activate Keyboard
  dataPort.Read();
}

MouseDriver::~MouseDriver() {

}

void printf(char* str);

uint32_t MouseDriver::HandleInterrupt(uint32_t esp) {
  uint8_t status = commandPort.Read();
  if(!(status & 0x20)) {
    return esp;
  }

  static int8_t x=40, y=12;

  buffer[offset] = dataPort.Read();
  offset = (offset + 1) % 3;

  uint16_t* VideoMemory = (uint16_t*)0xb8000;
  
  if(offset == 0) {
    
    VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4)
      | ((VideoMemory[80 * y + x] & 0x0F00) << 4)
      | (VideoMemory[80 * y + x] & 0x00FF);
    x += buffer[1];

    if (x < 0) x = 0;
    if (x >= 80) x = 79;
    
    y -= buffer[2];
    if (y < 0) y = 0;
    if (y >= 25) y = 24;
    VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4)
      | ((VideoMemory[80 * y + x] & 0x0F00) << 4)
      | (VideoMemory[80 * y + x] & 0x00FF);
    
  }
 
  
  return esp;
}
