#pragma once
#include "Event.h"
#include "..\Rest\Restaurant.h"
class CancelationEvent : public Event
{
private:

	//

public:
	CancelationEvent(int EvTime, int OrdID);
	bool Execute(Restaurant* pRest);

};