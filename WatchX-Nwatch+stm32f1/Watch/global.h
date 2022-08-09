/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_
#include "common.h"



void global_init(void);
void global_update(void);
byte div10(byte);
byte mod10(byte);

#endif /* GLOBAL_H_ */
