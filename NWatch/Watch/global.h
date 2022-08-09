#ifndef GLOBAL_H_
#define GLOBAL_H_
#include "common.h"

extern u8 UDADDR;

void global_init(void);
void global_update(void);
void detect_usb(void);

byte div10(byte);
byte mod10(byte);

#endif /* GLOBAL_H_ */
