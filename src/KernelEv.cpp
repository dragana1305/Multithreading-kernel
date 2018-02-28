#include "KernelEv.h"
#include "IVTEntry.h"
#include "Common.h"
#include "Schedule.h"
#include "PCB.h"
#include "List.h"

KernelEv::KernelEv(IVTNo ivtNo) {
	this->ivtNo = ivtNo;
	creator = running;	
	IVTEntry::ivtTable[ivtNo]->myEvent = this;
	flag = 2;
}

KernelEv::~KernelEv() {
	signal();
	IVTEntry::ivtTable[ivtNo]->myEvent = 0;
}

void KernelEv::wait() {
	if (running == creator) {
		flag = 1;
		creator->state = PCB::BLOCKED;
		dispatch();
	}
}

void KernelEv::signal() {
	if (flag == 1) { 
		flag = 2;
		Scheduler::put(creator);
	    creator->state = PCB::READY;
	}
}

