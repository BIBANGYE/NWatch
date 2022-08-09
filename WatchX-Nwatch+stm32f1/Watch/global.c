/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"

//static time_s chargeFinishTime;

void global_init()
{

}

void global_update()
{

}


// Dividing by 10 and mod 10 is done a lot in the code.
// Putting them in their own functions makes the code smaller, but slightly slower.
byte div10(byte val)
{
    return val / 10;
}

byte mod10(byte val)
{
    return val % 10;
}
