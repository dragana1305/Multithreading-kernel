#include "Dispatch.h"
#include "kerSem.h"
#include "iostream.h"
#include "Common.h"


unsigned tsp, tss, tbp;
pInterrupt oldISR;
PCB* oldRunning = 0;
int i;
unsigned spDiff;
int addSpDiff;
unsigned* bpPtr;

void interrupt timer(...) {
	if (!requiredContextSwitch && !requiredFork && counter > 0)
		counter--;
	if (requiredFork) {
		requiredFork = 0;
		
		if (forkChild == 0)
			return;
		
		asm {
			mov tss, ss
			mov tsp, sp
			mov tbp, bp
		}
		
		for (i = 0; i < forkChild->stackSize; i++) {
			forkChild->stack[i] = running->stack[i];
		}
		
		if (FP_OFF(forkChild->stack) > FP_OFF(running->stack)) {
			addSpDiff = 0;
			spDiff = FP_OFF(forkChild->stack) - FP_OFF(running->stack);
		}
		else {
			addSpDiff = 1;
			spDiff = FP_OFF(running->stack) - FP_OFF(forkChild->stack);
		}
		
		if (addSpDiff)
			tbp += spDiff;
		else
			tbp -= spDiff;
		
		forkChild->ss = FP_SEG(forkChild->stack);
		forkChild->sp = tbp;
		forkChild->bp = tbp;
		
		bpPtr = (unsigned*)MK_FP(FP_SEG(forkChild->stack), tbp);
		while (*bpPtr != 0) {
			if (addSpDiff)
				*bpPtr = *bpPtr + spDiff;
			else
				*bpPtr = *bpPtr - spDiff;
			
			bpPtr = (unsigned*)MK_FP(FP_SEG(forkChild->stack), *bpPtr);
		}
		
		return;
	}
	if (requiredContextSwitch || (running->timeSlice > 0 && counter == 0)) {
		if (lockFlag) {
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}
			
			running->sp = tsp;
			running->ss = tss;
			running->bp = tbp;
			
			if (running->state == PCB::READY && running!=idle->getPCB())
				Scheduler::put((PCB*)running);
			
			oldRunning = running;
			running = Scheduler::get();
			if (running == 0) {
				running = idle->getPCB();
			}
			counter = running->timeSlice;
			
			tsp = running->sp;
			tss = running->ss;
			tbp = running->bp;
			
			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
			
			if (oldRunning != 0 && oldRunning->type == PCB::FORKED && oldRunning->state == PCB::FINISHED) {
				delete oldRunning->myThread; oldRunning = 0;
			}
		}
	}
	if (!requiredContextSwitch && !requiredFork) {
		if (allBlocked != 0){
			static List::Elem *curr;
			curr = allBlocked->head;
			while (curr != 0) {
				static PCB* t;
				t = curr->pcb;
				curr = curr->next;
				t->timeToWait--;
				if (t->timeToWait == 0) {
					t->mySem->blocked->remove(t);
					allBlocked->remove(t);
					t->retValue = 0;
					Scheduler::put(t);
					t->state = PCB::READY;
					t->mySem->value++;
					t->mySem = 0;
				}
			}
		}
		tick();
		asm int 60h; 
	}
	else if (lockFlag)
		requiredContextSwitch = 0;
}



void inic(){
	lockInterrupt
	oldISR = getvect(0x8);
	setvect(0x8, timer);
	setvect(0x60, oldISR);
	unlockInterrupt
}


void restore(){
	lockInterrupt
	setvect(0x8, oldISR);
	unlockInterrupt
}
