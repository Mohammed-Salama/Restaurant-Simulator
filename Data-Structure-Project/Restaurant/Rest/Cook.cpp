#include "Cook.h"


Cook::Cook()
{
	busy = 0;
	Urgent = false;
	Break = 0;
	OrdersBeforeBreak = 0;
}


Cook::~Cook()
{
}

void Cook::setInitialOrdersBeforeBreak(int a)
{
	initialOrdersBeforeBreak = a;
}


int Cook::GetID() const
{
	return ID;
}


ORD_TYPE Cook::GetType() const
{
	return type;
}

void Cook::setIsInj(bool z)
{
	IsInj = z;
}


void Cook::setID(int id)
{
	ID = id;
}

void Cook::setType(ORD_TYPE t)
{
	type = t;
}

void Cook::setSpeed(int s)
{
		speed = s;
}

int Cook::getOrdersBeforeBreak()
{
	if (OrdersBeforeBreak == 0)
	{
		OrdersBeforeBreak= initialOrdersBeforeBreak;
		return 0;
	}
	else
	{
		OrdersBeforeBreak--;
		return OrdersBeforeBreak+1;
	}
}

int Cook::getSpeed()
{
	return speed;
}

bool Cook::isInjured()
{
	return IsInj;
}

void Cook::setbusy(int B)
{
	busy = B;
}

bool Cook::Isbusy()
{
	if (busy <= 0)
		return false;
	else
		busy--;
	return true;
}

void Cook::SetBreak(int b)
{
	Break = b;
}

void Cook::SetUrgent(bool U)
{
	Urgent = U;
}

bool Cook::isUrgent()
{
	return Urgent;
}

bool Cook::IsInBreak()
{
	if (Break <= 0)
		return false;
	else
		Break--;
	return true;
}


void Cook::SetOrder(Order* O,int a)
{
	busy = O->GetSize() / speed;
	if (O->GetSize() % speed != 0)
		busy++;
	O->SetFinishTime(busy+a);
	O->setServTime(busy);
}


