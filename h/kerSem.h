#ifndef _KERSEM_H
#define _KERSEM_H

class List;
typedef unsigned int Time;

class KernelSem {	
public: 
    int value;
	List *blocked;

    KernelSem(int init=1);
	~KernelSem();
	
	int wait(Time maxTimeToWait);
    void signal();
	
	void block(Time mTime);
    void deblock();
	
	int val() const { return value; }
	
};

#endif