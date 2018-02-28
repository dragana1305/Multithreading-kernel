#ifndef _DISPATCH_H_
#define _DISPATCH_H_

#include "dos.h"
#include "Common.h"
#include "Schedule.h"

typedef void interrupt (*pInterrupt)(...); 
extern pInterrupt oldISR;

void interrupt timer(...);
void inic();
void restore();
void tick();

#endif