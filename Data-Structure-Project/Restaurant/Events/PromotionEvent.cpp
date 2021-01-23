#include "PromotionEvent.h"
#include "../Rest/Restaurant.h"
PromotionEvent::PromotionEvent(int eTime, int oID, double exMoney) :Event(eTime, oID)
{
	ExMoney = exMoney;
}

bool PromotionEvent::Execute(Restaurant* pRest)
{
	Order* Found = pRest->RemovefromSuitableQueue(OrderID,TYPE_NRM);

	if (Found)
	{
		Found->SetOrderType(TYPE_VIP);
		Found->SetTotalMoney(ExMoney + Found->GetTotalMoney());
		pRest->AddtoSuitableQueue(Found);  // will be placed by "ArrivalTime" in Vip queue
	}

	return Found;
}

/*


Queue<Order*> Optr;
	Queue<Order*> OVptr;
	Queue<Order*> temp;
	Optr=pRest->getNormalOrders();
	OVptr = pRest->getVIPOrders();
	bool test = false;
	///If it is a priority queue I will dequeue only till I find the the order which i need , otherwise I will do LOOK AT THE CODE :'''D
	///And i Don't think that it is important
	int ID = OrderID;
	Order* ptr;
	Order* ToPromote;
	while (Optr.dequeue(ptr))
	{
		if (ID == ptr->GetID())
		{
			test = true;
			ToPromote = ptr;
		}
		else
		{
			temp.enqueue(ptr);
		}
	}
	/// if we use priority for normal the next while will be needed
	/*while (temp.dequeue(ptr)) // If the operator == is overloaded , we can only write Optr=temp instead of this while
	{
		Optr.enqueue(ptr);
	}*/
//if (test)
//{
//	ToPromote->increaseMoney(ExMoney);
//	ToPromote->setType(TYPE_VIP);
//	OVptr.enqueue(ToPromote); // I assumed That VIP Queue is a priority Queue , so that ToPromote Will be inserted at the right position
//}
//pRest->setVIPOrders(OVptr);
///// Incase of normal is priority temp in the next line must be Optr
//pRest->setNormalOrders(temp);


