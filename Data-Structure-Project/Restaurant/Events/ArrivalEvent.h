#ifndef __ARRIVAL_EVENT_H_
#define __ARRIVAL_EVENT_H_

#include "Event.h"


//class for the arrival event
class ArrivalEvent: public Event
{
	//info about the order ralted to arrival event
	int VIP_WUT; //Urgent Orders
	int OrdSize;	//order size
	ORD_TYPE OrdType;	//order type: Normal, vegan, VIP	                
	double OrdMoney;	//Total order money
public:
	ArrivalEvent(int eTime, int oID, ORD_TYPE oType,int oSize,double OMoney, int VipUr);
	//Add more constructors if needed
	ORD_TYPE GetType();
	
	virtual bool Execute(Restaurant *pRest);	//override execute function

};

#endif