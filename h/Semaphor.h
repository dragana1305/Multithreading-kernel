#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

typedef unsigned int Time;

class KernelSem;

class Semaphore {
public:
 Semaphore (int init=1);
 virtual ~Semaphore ();
 
 virtual int wait (Time maxTimeToWait);
 virtual void signal();
 
 int val () const; // Returns the current value of the semaphore

private:
 KernelSem* myImpl;
};



#endif