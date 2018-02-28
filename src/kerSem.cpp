#include "kerSem.h"
#include "Common.h"
#include "Schedule.h"
#include "iostream.h"

KernelSem::KernelSem(int init) : value(init) {
	blocked = new List();
}

KernelSem::~KernelSem() {
	List::Elem* curr = blocked->head;
	while (curr != 0) {
		signal();
		curr = curr->next;
	}
	delete blocked; 
}

int KernelSem::wait(Time maxTimeToWait) {
	if (--value < 0)
		block(maxTimeToWait);
	else 
		running->retValue = 1;
	return running->retValue;
}

void KernelSem::signal() {
	if (value++ < 0)
		deblock();
}

void KernelSem::block(Time mTime) {
	blocked->putElem(running);
	allBlocked->putElem(running);
	running->state = PCB::BLOCKED;
	running->timeToWait = mTime;
	running->mySem = this;
	dispatch();
}

void KernelSem::deblock() {
	PCB* t = blocked->getElem();
	allBlocked->remove(t);
	t->retValue = 1;
	t->mySem = 0;
	Scheduler::put(t);
	t->state = PCB::READY;
}

