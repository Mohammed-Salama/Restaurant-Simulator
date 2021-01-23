#pragma once
#include "../Events/Event.h"
class PromotionEvent : public Event
{			                
	double ExMoney;	
public:
	PromotionEvent(int eTime, int oID, double exMoney);
	
	virtual bool Execute(Restaurant *pRest);
};

