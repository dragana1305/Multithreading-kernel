#include "List.h"

List::~List() {
	while (head != 0) {
		List::Elem* old = head;
		head = head->next;
		delete old->pcb;
		delete old;
	}
}

void List::putElem(PCB *pcb) {
	List::Elem *newElem = new Elem(pcb, head);
	head = newElem;
	if (tail == 0) 
		tail = newElem;
}

PCB* List::getElem() {
	if (head == 0) 
		return 0;
	List::Elem *old = head;
	head = head->next;
	if (head == 0) 
		tail = 0;
	PCB* t = old->pcb;
	delete old;
	return t;
}

void List::remove(PCB* pcb) {
	List::Elem *curr = head, *prev = 0;
	while (curr && curr->pcb != pcb) {
		prev = curr;
		curr = curr->next;
	}
	if (curr == 0) 
		return;
	if (prev == 0) {
		head = head->next;
		if (head == 0) 
			tail = 0;
	}
	else {
		prev->next = curr->next;
		if (tail == curr) 
			tail = prev;
	}
	delete curr;
}


PCB* List::search(ID id) {
	for (List::Elem* curr = head; curr; curr = curr->next) 
		if (curr->pcb && curr->pcb->id == id) 
			return curr->pcb;
	return 0;
}