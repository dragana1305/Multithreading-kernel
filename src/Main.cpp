#include "Common.h"
#include "Dispatch.h"
#include "Main.h"

int main(int argc, char* argv[]) {
	
	lockDispatch 
	PCBlist = new List();
	allBlocked = new List();
	PCB* mainPCB = new PCB();
	mainPCB->started = 1;
	mainPCB->state = PCB::READY;
	running = mainPCB;
	idle = new Idle();
	idle->getPCB()->allocStack();
	counter = running->timeSlice;
	PCBlist->putElem(mainPCB);
	Main* mainThr = new Main(argc, argv);
	mainThr->start();
	unlockDispatch
	
	inic();
	mainThr->waitToComplete();
	restore();
	
	int val = mainThr->val;
	delete mainThr;
	delete idle; 
	PCBlist->remove(mainPCB);
	delete mainPCB;
	delete allBlocked;
	delete PCBlist;
	
	
	return val;
}