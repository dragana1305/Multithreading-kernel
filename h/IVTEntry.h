#ifndef _IVTENTRY_H_
#define _IVTENTRY_H_


typedef void interrupt (*pInterrupt)(...);
typedef unsigned char IVTNo;
class KernelEv;

class IVTEntry {
public:
	static IVTEntry* ivtTable[];
	KernelEv *myEvent;

	IVTNo ivtNo;
	pInterrupt oldRoutine;
	
	IVTEntry(int,pInterrupt);
	~IVTEntry();
	
	void signal();
	void callOldRoutine();
	
	IVTEntry *getIVTEntry() { return ivtTable[ivtNo]; }
	
};





#endif