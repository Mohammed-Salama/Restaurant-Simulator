#include "Order.h"

Order::Order(int id, ORD_TYPE r_Type, int arrTime, int size, double TotalMoney, int vipUrgent)
{
	VIP_UT = vipUrgent;
	ID = (id>0&&id<1000)?id:0;	//1<ID<999
	type = r_Type;
	status = WAIT;
	Size = size;
	ArrTime = arrTime;
	totalMoney = TotalMoney;
}

Order::~Order()
{
}

int Order::GetID()
{
	return ID;
}


ORD_TYPE Order::GetType() const
{
	return type;
}
void Order::SetOrderType(ORD_TYPE Type)
{
	type = Type;

}
void Order::SetTotalMoney(double TotMony)
{
	totalMoney = TotMony;
}
double Order::GetTotalMoney()
{
	return totalMoney;
}

void Order::SetSize(int d)
{
	Size = d > 0 ? d : 0;
}

int Order::GetSize() const
{
	return Size;
}

void Order::setStatus(ORD_STATUS s)
{
	status = s;
}

ORD_STATUS Order::getStatus() const
{
	return status;
}

int Order::getTime()
{
	return ArrTime;
}

int Order::GetUrgentTimeStep()
{
	return VIP_UT + ArrTime;
}

void Order::SetFinishTime(int f)
{
	FinishTime = f;
}

int Order::getFinishTime()
{
	return FinishTime;
}

void Order::setServTime(int a)
{
	ServTime = a;
}
int Order::getServTime()
{
	return ServTime;
}