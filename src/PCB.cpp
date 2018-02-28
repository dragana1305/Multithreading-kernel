#include "PCB.h"
#include "Common.h"
#include "dos.h"
#include "Schedule.h"
#include "Dispatch.h"
#include "kerSem.h"

ID PCB::totalId = 0;

void PCB::start() {
	if (!started) {
		state = READY;
		Scheduler::put(this);
		started = 1;
		
		allocStack();
	}
}

PCB::PCB(StackSize stackSize, Time timeSlice, Thread *myThread) {
	this->timeSlice = timeSlice;
	this->stackSize = stackSize / sizeof(unsigned);
	this->myThread = myThread;
	state = READY;
	started = 0;
	id = ++totalId;
	waitingList = new List();
	retValue = 0;
	timeToWait = 0;
	mySem = 0;
	type = NORMAL;
	numOfChildren = 0;
	parent = 0;
	semForChildren = new KernelSem(0);
	stack = 0;
}

void PCB::allocStack() {
	stack = new unsigned[stackSize];
	stack[stackSize - 1] = 0x200;
	stack[stackSize - 2] = FP_SEG(&wrapper); 
	stack[stackSize - 3] = FP_OFF(&wrapper);
	stack[stackSize - 12] = 0;
	ss = FP_SEG(stack + stackSize - 12);
	sp = FP_OFF(stack + stackSize - 12);
	bp = sp;
}

void PCB::waitToComplete() {
	lockDispatch 
	if (state != FINISHED && started != 0) { 
		waitingList->putElem(running); 
	    running->state = PCB::BLOCKED;
	    unlockDispatch
	    dispatch();
	}
	else unlockDispatch
}


PCB::~PCB() {
	delete semForChildren;
	delete waitingList;
	delete[] stack;
}



void PCB::wrapper() {
	running->myThread->run();
	exitThread();
}

void PCB::exitThread() { 
	lockDispatch
	running->state = PCB::FINISHED;
	PCB* pcb = running->waitingList->getElem();
	while (pcb) {
		pcb->state = PCB::READY;
		Scheduler::put(pcb);
		pcb = running->waitingList->getElem();
	}
	if (running->type == PCB::FORKED) {
		running->parent->numOfChildren--;
		if (running->parent->numOfChildren == 0 && running->parent->semForChildren->val() < 0)
			running->parent->semForChildren->signal();
	}
 	unlockDispatch
	dispatch();
}



