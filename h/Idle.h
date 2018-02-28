#ifndef _IDLE_H_
#define _IDLE_H_

#include "Thread.h"
#include "PCB.h"

class Idle : public Thread {
public:
	Idle() : Thread(defaultStackSize, 1) {}
	void run() {
		while (1) { }
    }
	virtual ~Idle () { waitToComplete(); }
	PCB* getPCB() { return myPCB; }
};


#endif