#ifndef __ORDER_H_
#define __ORDER_H_

#include "..\Defs.h"

class Order
{

protected:
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, vegan, VIP
	ORD_STATUS status;	//waiting, in-service, done
	                
	double totalMoney;	//Total order money

	int ArrTime, ServTime, FinishTime,Size, VIP_UT;	//arrival, service start, and finish times,Urgent time step
	
	//
	// TODO: Add More Data Members As Needed
	//

public:
	Order(int id, ORD_TYPE r_Type, int arrTime,int size, double TotalMoney,int vipUrgent);
	virtual ~Order();

	int GetID();

	ORD_TYPE GetType() const;
	void SetSize(int d);
	int GetSize() const;
	void SetOrderType(ORD_TYPE Type);
	void SetTotalMoney(double TotMony);
	double GetTotalMoney();
	void setStatus(ORD_STATUS s);
	ORD_STATUS getStatus() const;
	int getTime();
	int GetUrgentTimeStep();
	void SetFinishTime(int f);
	int getFinishTime();
	void setServTime(int a);
	int getServTime();
	//
	// TODO: Add More Member Functions As Needed
	//

};

#endif