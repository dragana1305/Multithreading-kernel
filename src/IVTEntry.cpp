#include "IVTEntry.h"
#include "dos.h"
#include "Common.h"
#include "KernelEv.h"

IVTEntry* IVTEntry::ivtTable[256] = {0};
typedef void interrupt (*pInterrupt)(...);

IVTEntry::IVTEntry(int ivtNo, pInterrupt newRoutine) {
	lockInterrupt
	myEvent = 0; 
	this->ivtNo = ivtNo;
	ivtTable[ivtNo] = this;
	oldRoutine = getvect(ivtNo);
	setvect(ivtNo, newRoutine);
	unlockInterrupt
}

IVTEntry::~IVTEntry() {
	lockInterrupt
	setvect(ivtNo, oldRoutine);
	unlockInterrupt
}
void IVTEntry::signal() {
	lockInterrupt
	if (myEvent)
		myEvent->signal();
	unlockInterrupt
}
void IVTEntry::callOldRoutine() {
	lockInterrupt
	if (oldRoutine)
		oldRoutine();
	unlockInterrupt
}

