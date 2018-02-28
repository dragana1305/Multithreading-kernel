#include "Common.h"


int lockFlag = 1;
int requiredContextSwitch = 0;
int requiredFork = 0;
PCB* running = 0;
Idle* idle = 0;
PCB* idlePCB = 0;
int counter = 0;
List* PCBlist = 0; 
List* allBlocked = 0;
PCB* forkChild = 0;

