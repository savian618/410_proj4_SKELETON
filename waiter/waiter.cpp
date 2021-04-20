#include <string>
#include "stdlib.h"

#include "../includes/waiter.h"
#include "../includes/baker.h"
#include "../includes/externs.h"

using namespace std;

Waiter::Waiter(int id,std::string filename):id(id),myIO(filename){
}

Waiter::~Waiter()
{
}

//gets next Order(s) from file_IO
int Waiter::getNext(ORDER &anOrder){
	int n = myIO.getNext(anOrder);
	if(n != SUCCESS){
		return NO_ORDERS;
	}
	return SUCCESS;
}

void Waiter::beWaiter() {
	ORDER o;
	while(getNext(o) == SUCCESS){
		lock_guard<mutex> lck(mutex_order_inQ);
		order_in_Q.push(o);
	}
	b_WaiterIsFinished = true;
	cv_order_inQ.notify_all();
}

