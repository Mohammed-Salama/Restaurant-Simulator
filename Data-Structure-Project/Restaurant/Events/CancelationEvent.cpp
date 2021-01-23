#include "CancelationEvent.h"
CancelationEvent::CancelationEvent(int EvTime, int OrdID) :Event(EvTime, OrdID)
{

}


bool CancelationEvent::Execute(Restaurant* pRest)
{
	Order* canceled = pRest->RemovefromSuitableQueue(OrderID, TYPE_NRM);
	if (canceled)
		delete canceled;
	else
		return 0;
	return 1;

}