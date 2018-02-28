#include "Event.h"
#include "Common.h"
#include "KernelEv.h"

Event::Event(IVTNo ivtNo) {
	lockInterrupt
	myImpl = new KernelEv(ivtNo);
	unlockInterrupt
}

Event::~Event() {
	lockInterrupt
	delete myImpl;
	unlockInterrupt
}

void Event::wait() {
	lockInterrupt
	myImpl->wait();
	unlockInterrupt
}

void Event::signal() {
	lockInterrupt
	myImpl->signal();
	unlockInterrupt
}
