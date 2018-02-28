#ifndef _LIST_H_
#define _LIST_H_

#include "PCB.h"

class List {
public:
   struct Elem {
	   PCB *pcb;
	   Elem *next;
	   
	   Elem(PCB *pcb = 0, Elem *next = 0) {
		   this->pcb = pcb;
		   this->next = next;
	   }
   };
   Elem *head, *tail;
   
   List() { head = tail = 0; }
   ~List();
   void putElem(PCB*);
   PCB* getElem();
   void remove(PCB*); 
   PCB* search(ID id);
};


#endif