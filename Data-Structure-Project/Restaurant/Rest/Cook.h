#pragma once
#include"Order.h"
#include "..\Defs.h"
#include <cstdlib>
#include <time.h>

#pragma once
class Cook
{
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding type (VIP, Normal, Vegan)
	int speed;		//dishes it can prepare in one clock tick (in one timestep)
	bool IsInj;
	int busy; //time steps of busyness in Break or in Servace
	int initialOrdersBeforeBreak;
	int OrdersBeforeBreak; //how many orders before the Break 
	bool Urgent;
	int Break;
public:
	Cook();
	virtual ~Cook();
	void setInitialOrdersBeforeBreak(int a);
	int GetID() const;
	ORD_TYPE GetType() const;
	void setIsInj(bool);
	void setID(int);
	void setType(ORD_TYPE) ;
	void setSpeed(int s);
	int getOrdersBeforeBreak();
	int getSpeed();
	bool isInjured();
	void setbusy(int B);
	bool Isbusy();
	void SetBreak(int b);
	void SetUrgent(bool U);
	bool isUrgent();
	bool IsInBreak();
	void SetOrder(Order*O,int a);
};
