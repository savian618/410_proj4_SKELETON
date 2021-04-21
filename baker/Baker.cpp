#include <mutex>

#include "../includes/baker.h"
#include "../includes/externs.h"
using namespace std;

Baker::Baker(int id):id(id)
{
}

Baker::~Baker()
{
}

void Baker::bake_and_box(ORDER &anOrder) {
	Box b;
	int numD = anOrder.number_donuts;
	
	for(int i = 0; i < numD; i++){
		DONUT d;
		b.addDonut(d);
		if(b.size() == DOZEN){
			anOrder.boxes.push_back(b);
			b.clear();
		}
	}
	if(b.size() > 0){
		anOrder.boxes.push_back(b);
		b.clear();
	}
}

void Baker::beBaker() {
	while(true){
		unique_lock<mutex> lck(mutex_order_inQ);
		ORDER o;
		if(b_WaiterIsFinished){
			while(order_in_Q.size() > 0){
				o = order_in_Q.front();
				order_in_Q.pop();
				bake_and_box(o);
				lock_guard<mutex> lk(mutex_order_outQ);
				order_out_Vector.push_back(o);
			}
			break;
		}
		while(order_in_Q.size() > 0 && !b_WaiterIsFinished){
			cv_order_inQ.wait(lck);

		}
		o = order_in_Q.front();
		order_in_Q.pop();
		bake_and_box(o);
		lock_guard<mutex> lk(mutex_order_outQ);
		order_out_Vector.push_back(o);

	}
}
