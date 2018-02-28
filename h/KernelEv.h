#ifndef _KernelEv_H_
#define _KernelEv_H_

typedef unsigned char IVTNo;
class PCB;

class KernelEv {
public:
	KernelEv(IVTNo);
	~KernelEv(); 
	
	void wait();
	void signal();
	
	PCB* creator;
	IVTNo ivtNo;
	int flag; 

};



#endif
