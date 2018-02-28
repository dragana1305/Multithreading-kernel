#ifndef _PCB_H_
#define _PCB_H_

#include "Thread.h"
class List;
class Idle;
class KernelSem;

class PCB {
public:
	PCB(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice, Thread *myThread = 0);   
	~PCB();
	
	void start();
	void waitToComplete();
	
	static void wrapper();
    static void exitThread();
	
	void allocStack();

    Thread *myThread;
    unsigned *stack;
	StackSize stackSize;
    unsigned sp, ss, bp;
 	Time timeSlice;
	
	enum State {READY, BLOCKED, FINISHED};
	State state;
	int started;
	
	static ID totalId;
	ID id;
	
	List* waitingList;
	
	int retValue;
	int timeToWait;
	KernelSem *mySem;
	
	enum Type {NORMAL, FORKED};
	Type type;
	int numOfChildren;
	PCB* parent;
	KernelSem* semForChildren;
};


#endif