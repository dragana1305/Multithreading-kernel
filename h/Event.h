#ifndef _event_h_ 
#define _event_h_  


typedef unsigned char IVTNo;

class KernelEv;  

#include "IVTEntry.h"

#define PREPAREENTRY(numEntry, callOld) \
void interrupt inter##numEntry(...); \
IVTEntry newEntry##numEntry(numEntry, inter##numEntry); \
void interrupt inter##numEntry(...) { \
	if (callOld == 1) \
		newEntry##numEntry.callOldRoutine(); \
	newEntry##numEntry.signal(); \
	dispatch(); \
}



class Event { 
public:    
	Event(IVTNo ivtNo);   
	~Event();  
	
	void wait(); 
 
protected:    
	friend class KernelEv;    
	void signal(); // can call KernelEv  

private:    
	KernelEv* myImpl; 
}; 




#endif 
//prepareentry