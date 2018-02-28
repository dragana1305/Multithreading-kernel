#include "Semaphor.h"
#include "Common.h"
#include "kerSem.h"


Semaphore::Semaphore(int init) {
	lockInterrupt
	myImpl = new KernelSem(init);
	unlockInterrupt
}

Semaphore::~Semaphore () {
	lockInterrupt
	delete myImpl;
	unlockInterrupt
}

int Semaphore::wait(Time maxTimeToWait) {
    lockInterrupt   
	int retValue = myImpl->wait(maxTimeToWait);
	unlockInterrupt
	return retValue;
}

void Semaphore::signal() {
	lockInterrupt
	myImpl->signal();
	unlockInterrupt
}

int Semaphore::val() const {
	return myImpl->val();
}
