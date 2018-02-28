#ifndef _MAIN_H_
#define _MAIN_H_

#include "thread.h"

int userMain(int, char*[]);

class Main : public Thread {
public:
	Main(int n, char** param) {
		this->n = n;
		this->param = param;
	}
	
	virtual ~Main() {
		waitToComplete();
	}
	
	void run() {
		val = userMain(n, param);
	}
	
	Thread* clone() const {
		return new Main(n, param);
	}
	
	int n, val;
	char** param;
};

#endif