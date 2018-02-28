#include "Thread.h"
#include "PCB.h"
#include "Dispatch.h"
#include "Common.h"
#include "kerSem.h"

ID Thread::fork() {
	lockInterrupt
	Thread* thr = running->myThread->clone();
	if (thr == 0) {
		unlockInterrupt
		return -1;
	}
	PCB* pcb = thr->myPCB;
	pcb->stack = new unsigned[pcb->stackSize];
	if (pcb->stack == 0) {
		delete thr;
		unlockInterrupt
		return -1;
	}
	Scheduler::put(pcb);
	pcb->state = PCB::READY;
	pcb->type = PCB::FORKED;
	pcb->parent = running;
	running->numOfChildren++;
	running->retValue = pcb->id;
	pcb->retValue = 0;
	requiredFork = 1;
	forkChild = pcb;
	timer();
	forkChild = 0;
	unlockInterrupt
	return running->retValue;
}

void Thread::exit() {
	PCB::exitThread();
}

void Thread::waitForForkChildren() {
	lockDispatch
	if (running->numOfChildren > 0) {
		running->semForChildren->wait(0);
	}
	unlockDispatch
}

Thread* Thread::clone() const {
	return 0;
}

void Thread::start() {
	lockDispatch
	myPCB->start();
	unlockDispatch
}

void Thread::waitToComplete() {
	myPCB->waitToComplete(); 
}

Thread::~Thread() {
	waitToComplete(); 
	lockDispatch
	PCBlist->remove(myPCB);
	delete myPCB;
	unlockDispatch
}

ID Thread::getId() {
	lockDispatch
	ID id = myPCB->id;
	unlockDispatch
	return id;
}

ID Thread::getRunningId() {
	lockDispatch
	ID id = running->id;
	unlockDispatch
	return id;
}

Thread* Thread::getThreadById(ID id) {
	lockDispatch
	PCB* pcb = PCBlist->search(id);
	Thread *thread;
	if (pcb) thread = pcb->myThread;
	else thread = 0;
	unlockDispatch
	return thread;
}

Thread::Thread (StackSize stackSize, Time timeSlice) {
	lockDispatch
	myPCB = new PCB(stackSize, timeSlice, this);
	PCBlist->putElem(myPCB);
	unlockDispatch
}

void dispatch() {
	lockInterrupt 
	requiredContextSwitch = 1;
	timer();
	unlockInterrupt
}


