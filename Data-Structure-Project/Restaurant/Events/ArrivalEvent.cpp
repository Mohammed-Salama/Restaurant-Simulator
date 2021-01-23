#include "ArrivalEvent.h"
#include "..\Rest\Restaurant.h"


ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType, int oSize, double OMoney,int VipUr) :Event(eTime, oID)
{
	VIP_WUT = VipUr;
	OrdType = oType;
	OrdSize = oSize;
	OrdMoney = OMoney;
}

bool ArrivalEvent::Execute(Restaurant* pRest)
{
	Order* pOrd = new Order(OrderID, OrdType, EventTime, OrdSize, OrdMoney,VIP_WUT);
	pOrd->setStatus(WAIT);

	return pRest->AddtoSuitableQueue(pOrd);
}

ORD_TYPE ArrivalEvent::GetType()
{
	return OrdType;
}
