#ifndef _COMMON_H_
#define _COMMON_H_

#include "List.h"
#include "PCB.h"
#include "Thread.h"
#include "Idle.h"

extern int lockFlag; 
extern int requiredContextSwitch;
extern int requiredFork;

#define lockInterrupt asm { pushf;\
                   cli;\
                 }
#define unlockInterrupt asm popf;

#define lockDispatch lockFlag = 0;
#define unlockDispatch lockFlag = 1;\
               if (requiredContextSwitch) {\
				   dispatch();\
			   }

extern PCB* running;
extern Idle* idle;
extern PCB* idlePCB;
extern int counter;
extern List* PCBlist;
extern List* allBlocked;
extern PCB* forkChild;


#endif