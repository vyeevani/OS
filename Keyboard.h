#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "types.h"
#include "port.h"
#include "interrupts.h"


class KeyboardDriver: public InterruptHandler {
  Port8Bit dataPort;
  Port8Bit commandPort;

 public:
  KeyboardDriver(InterruptManager* interruptManager);
  ~KeyboardDriver();
  virtual uint32_t HandleInterrupt(uint32_t esp);
};


#endif
