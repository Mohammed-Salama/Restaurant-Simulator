#include <cstdlib>
#include <time.h>
#include <fstream>
#include<string>
#include <iomanip>

using namespace std;

#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"


bool VIPWeightFunc(Order* a, Order* b)
{
	return float(a->getTime()) * a->GetSize() / (a->GetTotalMoney() * a->GetTotalMoney()) < float(b->getTime()) * b->GetSize() / (b->GetTotalMoney() * b->GetTotalMoney());

}

bool FinishedFunc(Order* a, Order* b)
{
	if(a->getFinishTime()== b->getFinishTime())
		return a->getTime() < b->getTime();
	else
		return a->getFinishTime() < b->getFinishTime();

}


Restaurant::Restaurant()
{
	pGUI = NULL;
	num_wait_normal = 0;
	num_wait_vegan = 0;
	num_wait_vip = 0;
	mts = 500;
}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE	mode = pGUI->getGUIMode();

	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		Simulate(1);
		break;
	case MODE_STEP:
		Simulate(2);
		break;
	case MODE_SLNT:
		Finalize();
	};
}

//////////////////////////////////  Event handling functions   /////////////////////////////

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	bool succeeded = false;
	Event* pE;
	while (EventsQueue.peekFront(pE))	//as long as there are more events
	{
		if (pE->getEventTime() > CurrentTimeStep)	//no more events at current timestep
			return;


		// to update the number of waiting orders
		succeeded = pE->Execute(this);
		if (succeeded)
		{
			if (dynamic_cast<PromotionEvent*>(pE))
			{
				num_wait_normal--;
				num_wait_vip++;
				num_norm_orders--;
				num_vip_orders++;
			}
			else if (dynamic_cast<CancelationEvent*>(pE))
			{
				num_wait_normal--;
				num_norm_orders--;
			}
			else
			{
				ORD_TYPE type;
				ArrivalEvent* pAE = (ArrivalEvent*)pE;
				type = pAE->GetType();
				if (type == TYPE_NRM)
					num_wait_normal++;
				else if (type == TYPE_VGAN)
					num_wait_vegan++;
				else
					num_wait_vip++;
			}
		}
		EventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory
	}
}


Restaurant::~Restaurant()
{
	if (pGUI)
		delete pGUI;
}

void Restaurant::FillDrawingList()
{
	Queue <Cook*>CookTempQueue;
	Queue <Order*>OrderTempQueue;
	while (!VipCooksQueue.isEmpty())
	{
		Cook* tempCook;
		VipCooksQueue.dequeue(tempCook);
		CookTempQueue.enqueue(tempCook);
		pGUI->AddToDrawingList(tempCook);
	}
	while (!CookTempQueue.isEmpty())
	{
		Cook* tempCook;
		CookTempQueue.dequeue(tempCook);
		VipCooksQueue.enqueue(tempCook);
	}
	/////////////////////////////////////
	while (!VeganCooksQueue.isEmpty())
	{
		Cook* tempCook;
		VeganCooksQueue.dequeue(tempCook);
		CookTempQueue.enqueue(tempCook);
		pGUI->AddToDrawingList(tempCook);
	}
	while (!CookTempQueue.isEmpty())
	{
		Cook* tempCook;
		CookTempQueue.dequeue(tempCook);
		VeganCooksQueue.enqueue(tempCook);
	}
	///////////////////////////////////////
	while (!NormalCooksQueue.isEmpty())
	{
		Cook* tempCook;
		NormalCooksQueue.dequeue(tempCook);
		CookTempQueue.enqueue(tempCook);
		pGUI->AddToDrawingList(tempCook);
	}
	while (!CookTempQueue.isEmpty())
	{
		Cook* tempCook;
		CookTempQueue.dequeue(tempCook);
		NormalCooksQueue.enqueue(tempCook);
	}
	//////////////////////////////////////

	while (!NormalOrdersQueue.isEmpty())
	{
		Order* tempOrder;
		NormalOrdersQueue.dequeue(tempOrder);
		OrderTempQueue.enqueue(tempOrder);
		pGUI->AddToDrawingList(tempOrder);
	}
	while (!OrderTempQueue.isEmpty())
	{
		Order* tempOrder;
		OrderTempQueue.dequeue(tempOrder);
		NormalOrdersQueue.enqueue(tempOrder);
	}
	////////////////////////////////////////


	while (!VeganOrdersQueue.isEmpty())
	{
		Order* tempOrder;
		VeganOrdersQueue.dequeue(tempOrder);
		OrderTempQueue.enqueue(tempOrder);
		pGUI->AddToDrawingList(tempOrder);
	}
	while (!OrderTempQueue.isEmpty())
	{
		Order* tempOrder;
		OrderTempQueue.dequeue(tempOrder);
		VeganOrdersQueue.enqueue(tempOrder);
	}
	////////////////////////////////////////
	
	while (!VIPOrdersQueue.isEmpty())
	{
		Order* tempOrder;
		VIPOrdersQueue.dequeue(tempOrder);
		OrderTempQueue.enqueue(tempOrder);
		pGUI->AddToDrawingList(tempOrder);
	}

	while (!OrderTempQueue.isEmpty())
	{
		Order* tempOrder;
		OrderTempQueue.dequeue(tempOrder);
		VIPOrdersQueue.enqueue(tempOrder);
	}
	while (!UrgentVIPOrdersQueue.isEmpty())
	{
		Order* tempOrder;
		UrgentVIPOrdersQueue.dequeue(tempOrder);
		OrderTempQueue.enqueue(tempOrder);
		pGUI->AddToDrawingList(tempOrder);
	}
	while (!OrderTempQueue.isEmpty())
	{
		Order* tempOrder;
		OrderTempQueue.dequeue(tempOrder);
		UrgentVIPOrdersQueue.enqueue(tempOrder);
	}
	////////////////////////////////////////
	while (!InService.isEmpty())
	{
		Order* tempOrder;
		InService.dequeue(tempOrder);
		OrderTempQueue.enqueue(tempOrder);
		pGUI->AddToDrawingList(tempOrder);
	}
	while (!OrderTempQueue.isEmpty())
	{
		Order* tempOrder;
		OrderTempQueue.dequeue(tempOrder);
		InService.enqueue(tempOrder);
	}
	while (!Finished.isEmpty())
	{
		Order* tempOrder;
		Finished.dequeue(tempOrder);
		OrderTempQueue.enqueue(tempOrder);
		pGUI->AddToDrawingList(tempOrder);
	}
	while (!OrderTempQueue.isEmpty())
	{
		Order* tempOrder;
		OrderTempQueue.dequeue(tempOrder);
		Finished.enqueue(tempOrder);
	}


	//How to get orderNumbers
	//TODOThis function should be implemented in phase1
	//It should add ALL orders and Cooks to the drawing list
	//It should get orders from orders lists/queues/stacks/whatever (same for Cooks)
	//To add orders it should call function  void GUI::AddToDrawingList(Order* pOrd);
	//To add Cooks it should call function  void GUI::AddToDrawingList(Cook* pCc);
}




//////////////////////////////////////////////////////////////////////////////////////////////
/// ==> 
///  DEMO-related functions. Should be removed in phases 1&2

//Begin of DEMO-related functions

//This is just a demo function for project introductory phase
//It should be removed starting phase 1
////////////////

void Restaurant::AddtoDemoQueue(Order* pOrd)
{
	DEMO_Queue.enqueue(pOrd);
}

bool Restaurant::AddtoSuitableQueue(Order* pOrd)
{
	ORD_TYPE OrdType = pOrd->GetType();
	if (OrdType == TYPE_NRM)
		return NormalOrdersQueue.enqueue(pOrd);
	else if (OrdType == TYPE_VGAN)
		return VeganOrdersQueue.enqueue(pOrd);
	else
		return VIPOrdersQueue.enqueue(pOrd);
}

Order* Restaurant::RemovefromSuitableQueue(Order* pOrd)
{
	return RemovefromSuitableQueue(pOrd->GetID() , pOrd->GetType());
}

Order* Restaurant::RemovefromSuitableQueue(int id , ORD_TYPE OrdType)
{
	Queue<Order*>* PSuitableQueue;

	switch (OrdType)
	{
	case TYPE_NRM:
		PSuitableQueue = &NormalOrdersQueue;
		break;
	case TYPE_VGAN:
		PSuitableQueue = &VeganOrdersQueue;
		break;
	case TYPE_VIP:
		PSuitableQueue = &VIPOrdersQueue;
		break;
	default:
		break;
	}


	Queue<Order*> TempOrderQueue;
	bool Found = false;
	Order* pDequeuedOrder;
	Order* DeletedNormalOrder;
	Order* rtn = nullptr;

	while (PSuitableQueue->dequeue(pDequeuedOrder))
	{
		if (pDequeuedOrder->GetID() == id)
		{
			rtn = pDequeuedOrder;  // cancelation for the order
			Found = 1;
		}
		else
		{
			TempOrderQueue.enqueue(pDequeuedOrder);
		}
	}

	Order* TempNormalOrder;
	while (TempOrderQueue.dequeue(TempNormalOrder))  // return orders to the main order 
		PSuitableQueue->enqueue(TempNormalOrder);


	return rtn;
}
/// ==> end of DEMO-related function
//////////////////////////////////////////////////////////////////////////////////////////////

void Restaurant::ReadData()
{
	pGUI->PrintMessage("Please Enter input file name without .txt ");
	string S = pGUI->GetString();
	ifstream cinn;
	cinn.open(S+".txt");
	cinn >> NumNormCooks >> NumVeganCooks >> NumVipCooks >> NormSpeedMin >> NormSpeedMax >> VeganSpeedMin >> VeganSpeedMax >> VipSpeedMin >> VipSpeedMax;
	cinn >> NumBeforeBreak >> NormBerakMin >> NormBerakMax >> VegaBreakMin >> VegaBreakMax >> VipBreakMin >> VipBreakMax >> InjProp >> RstPrd >>AutoPor >> VIP_WT >>NumEvents;
	char CharTemp;//temperory Char
	int NumTemp1;//temperory number1
	int NumTemp2;//temperory number2
	int NumTemp3;//temperory number3
	int NumTemp4;//temperory number4
	num_veg_orders = 0;
	num_vip_orders = 0;
	num_norm_orders = 0;
	for (int i = 1; i <= NumEvents; i++)        //<=
	{
		cinn >> CharTemp;
		if (CharTemp == 'R') //Arrival
		{
			cinn >> CharTemp;
			cinn >> NumTemp1;
			cinn >> NumTemp2;
			cinn >> NumTemp3;
			cinn >> NumTemp4;
			if (CharTemp == 'N')
			{
				num_norm_orders++;
				ArrivalEvent* AE = new ArrivalEvent(NumTemp1, NumTemp2, TYPE_NRM, NumTemp3, NumTemp4, VIP_WT);
				EventsQueue.enqueue(AE);
			}
			else if (CharTemp == 'G')
			{
				num_veg_orders++;
				ArrivalEvent* AE = new ArrivalEvent(NumTemp1, NumTemp2, TYPE_VGAN, NumTemp3, NumTemp4, VIP_WT);
				EventsQueue.enqueue(AE);
			}
			else
			{
				num_vip_orders++;
				ArrivalEvent* AE = new ArrivalEvent(NumTemp1, NumTemp2, TYPE_VIP, NumTemp3, NumTemp4, VIP_WT);
				EventsQueue.enqueue(AE);
			}


		}
		else if (CharTemp == 'X') //Cancelation
		{
			cinn >> NumTemp1;
			cinn >> NumTemp2;
			CancelationEvent* CE = new CancelationEvent(NumTemp1, NumTemp2);
			EventsQueue.enqueue(CE);
		}
		else//Promotion
		{
			cinn >> NumTemp1;
			cinn >> NumTemp2;
			cinn >> NumTemp3;
			PromotionEvent* PE = new PromotionEvent(NumTemp1, NumTemp2, NumTemp3);
			EventsQueue.enqueue(PE);
		}
	}
	cinn.close();

	Cook* pCook;
	int Cookindex=1;
	for (int i = 0;i < NumVeganCooks;i++)
	{
		srand(time(NULL));
		pCook = new Cook;
		int v = VeganSpeedMax - VeganSpeedMin + 1;
		if (v <1)
			v = 1;
		pCook->setSpeed(VeganSpeedMin+(rand()%v));
		pCook->setInitialOrdersBeforeBreak(NumBeforeBreak);
		pCook->getOrdersBeforeBreak();
		pCook->setType(TYPE_VGAN);
		pCook->setID(Cookindex);
		VeganCooksQueue.enqueue(pCook);
		Cookindex++;
	}
	for (int i = 0;i < NumNormCooks;i++)
	{
		srand(time(NULL));
		pCook = new Cook;
		int v = NormSpeedMax + 1 - NormSpeedMin;
		if (v < 1)
			v = 1;
		pCook->setSpeed(NormSpeedMin+ (rand() % v));
		pCook->setInitialOrdersBeforeBreak(NumBeforeBreak);
		pCook->getOrdersBeforeBreak();
		pCook->setType(TYPE_NRM);
		pCook->setID(Cookindex);
		NormalCooksQueue.enqueue(pCook);
		Cookindex++;
	}
	for (int i = 0;i < NumVipCooks;i++)
	{
		srand(time(NULL));
		pCook = new Cook;
		int v;
		v = (VipSpeedMax + 1 - VipSpeedMin);
		if (v < 1)
			v = 1;
		pCook->setSpeed(VipSpeedMin + (rand() % v));
		pCook->setInitialOrdersBeforeBreak(NumBeforeBreak);
		pCook->getOrdersBeforeBreak();
		pCook->setType(TYPE_VIP);
		pCook->setID(Cookindex);
		VipCooksQueue.enqueue(pCook);
		Cookindex++;
	}

}
// i have done 2 edits in the function Above : 1-NumVipCooks 2-EventsQueue.enqueue(CE) 



void Restaurant::Simulate(int x)
{
	ReadData();
	Event* pEv;
	Cook* cook;
	Order* OptrN;
	Order* OptrVI;
	Order* OptrVG;
	Order* Optr;
	Queue <Order*>TempQueue;
	Queue <Cook*>TempQueueCooks;
	string OperationDone;// = "Cooks and assigned orders: ";//assignments done in this time step


	int CurrentTimeStep = 1;
	//pGUI->PrintMessage("Click to simulate");
	//pGUI->waitForClick();
	int num_serv_normal = 0;
	int num_serv_vegan = 0;
	int num_serv_vip = 0;
	int num_Cooks_normal = NumNormCooks;
	int	num_Cooks_vegan = NumVeganCooks;
	int	num_Cooks_vip = NumVipCooks;
	int total = 0;
	
	char numavailnrmcook[10];
	char numavailvgncook[10];          // to print at GUI
	char numavailvipcook[10];

	char numwaitingnrm[10];
	char numwatingvip[10];                 // to print at GUI
	char numwatingvgn[10];

	while (!EventsQueue.isEmpty() || !InService.isEmpty() || !NormalOrdersQueue.isEmpty() || !VeganOrdersQueue.isEmpty() || !VIPOrdersQueue.isEmpty() || !UrgentVIPOrdersQueue.isEmpty())
	{
		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step
		
		
		char timestep[10];

		itoa(CurrentTimeStep, timestep, 10);
		string display0(timestep);
		
		//pGUI->waitForClick();
		
		while (!NormalOrdersQueue.isEmpty())
		{
			NormalOrdersQueue.dequeue(Optr);
			if (CurrentTimeStep - Optr->getTime() >= AutoPor)
			{
				VIPOrdersQueue.enqueue(Optr);
				num_wait_vip++;
				num_wait_normal--;
			}
			else
			{
				TempQueue.enqueue(Optr);
			}
		}
		while (!TempQueue.isEmpty())
		{
			TempQueue.dequeue(Optr);
			NormalOrdersQueue.enqueue(Optr);
		}

		srand((unsigned)time(0));
		float randNum = (float)rand() / RAND_MAX;
		if (randNum <= InjProp)
		{
			if (!CooksInservace.isEmpty())
			{
				InService.dequeue(Optr);
				CooksInservace.dequeue(cook);
				cook->setIsInj(true);
				cook->setSpeed(ceil(float(cook->getSpeed()) / 2.0));
				cook->SetOrder(Optr,CurrentTimeStep);
				InService.enqueue(Optr);
			}
		}

		while (!CooksInBreak.isEmpty())
		{
			CooksInBreak.dequeue(cook);
			if (!cook->IsInBreak())
			{
				if (cook->isUrgent())
				{
					cook->setSpeed(cook->getSpeed() * 2);
					cook->SetUrgent(false);
				}
				if (cook->isInjured())
				{
					cook->setSpeed(cook->getSpeed() * 2);
					cook->setIsInj(false);
				}

				if (cook->GetType() == TYPE_VIP)
				{
					num_Cooks_vip++;
					VipCooksQueue.enqueue(cook);
				}
				else if (cook->GetType() == TYPE_NRM)
				{
					num_Cooks_normal++;
					NormalCooksQueue.enqueue(cook);
				}
				else if (cook->GetType() == TYPE_VGAN)
				{
					num_Cooks_vegan++;
					VeganCooksQueue.enqueue(cook);
				}
			}
			else
			{
				TempQueueCooks.enqueue(cook);
			}
		}
		while (!TempQueueCooks.isEmpty())
		{
			TempQueueCooks.dequeue(cook);
			CooksInBreak.enqueue(cook);
		}
		
		while (!CooksInservace.isEmpty())
		{
			CooksInservace.dequeue(cook);
			if (!cook->Isbusy())
			{
				if (cook->getOrdersBeforeBreak() == 0)
				{
					srand((unsigned)time(0));
					if (cook->GetType() == TYPE_VIP)
						cook->SetBreak(VipBreakMin + (rand() % (VipBreakMax + 1 - VipBreakMin)));
					if (cook->GetType() == TYPE_NRM)
						cook->SetBreak(NormBerakMin + (rand() % (NormBerakMax + 1 - NormBerakMin)));
					if (cook->GetType() == TYPE_VGAN)
						cook->SetBreak(VegaBreakMin + (rand() % (VegaBreakMax + 1 - VegaBreakMin)));
					CooksInBreak.enqueue(cook);
				}
				else
				{
					if (cook->GetType() == TYPE_VIP)
					{
						num_Cooks_vip++;
						VipCooksQueue.enqueue(cook);
					}
					else if (cook->GetType() == TYPE_NRM)
					{
						num_Cooks_normal++;
						NormalCooksQueue.enqueue(cook);
					}
					else if (cook->GetType() == TYPE_VGAN)
					{
						num_Cooks_vegan++;
						VeganCooksQueue.enqueue(cook);
					}
				}
			}
			else
			{
				TempQueueCooks.enqueue(cook);
			}
		}
		while (!TempQueueCooks.isEmpty())
		{
			TempQueueCooks.dequeue(cook);
			CooksInservace.enqueue(cook);
		}

		while (!InService.isEmpty())
		{
			InService.dequeue(Optr);
			if (Optr->getFinishTime() <= CurrentTimeStep)
			{
				if(Optr->GetType()== TYPE_VIP)
					num_serv_vip--;
				else if(Optr->GetType() == TYPE_NRM)
					num_serv_normal--;
				else
					num_serv_vegan--;
				Optr->setStatus(DONE);
				Finished.enqueue(Optr);
				total++;
			}
			else
			{
				TempQueue.enqueue(Optr);
			}
		}
		while (!TempQueue.isEmpty())
		{
			TempQueue.dequeue(Optr);
			InService.enqueue(Optr);
		}

		while (!VIPOrdersQueue.isEmpty())
		{
			VIPOrdersQueue.dequeue(Optr);
			if (Optr->GetUrgentTimeStep() <= CurrentTimeStep)
				UrgentVIPOrdersQueue.enqueue(Optr);
			else
				TempQueue.enqueue(Optr);
		}
		while (!TempQueue.isEmpty())
		{
			TempQueue.dequeue(Optr);
			VIPOrdersQueue.enqueue(Optr);
		}

		while (!UrgentVIPOrdersQueue.isEmpty())
		{
			if (!VipCooksQueue.isEmpty())
			{
				UrgentVIPOrdersQueue.dequeue(OptrVI);
				VipCooksQueue.dequeue(cook);
				OptrVI->setStatus(SRV);
				cook->SetOrder(OptrVI,CurrentTimeStep);
				InService.enqueue(OptrVI);
				CooksInservace.enqueue(cook);
				num_wait_vip--;
				num_serv_vip++;
				num_Cooks_vip--;
				OperationDone += "V" + to_string(cook->GetID()) + "(" + "V" + to_string(OptrVI->GetID()) + ")   ";
			}
			else if (!VeganCooksQueue.isEmpty())
			{
				UrgentVIPOrdersQueue.dequeue(OptrVI);
				VeganCooksQueue.dequeue(cook);
				OptrVI->setStatus(SRV);
				InService.enqueue(OptrVI);
				cook->SetOrder(OptrVI, CurrentTimeStep);
				CooksInservace.enqueue(cook);
				OperationDone += "G" + to_string(cook->GetID()) + "(" + "V" + to_string(OptrVI->GetID()) + ")   ";
				num_wait_vip--;
				num_Cooks_vegan--;
				num_serv_vip++;
			}
			else if (!NormalCooksQueue.isEmpty())
			{
				UrgentVIPOrdersQueue.dequeue(OptrVI);
				NormalCooksQueue.dequeue(cook);
				OptrVI->setStatus(SRV);
				InService.enqueue(OptrVI);
				cook->SetOrder(OptrVI, CurrentTimeStep);
				CooksInservace.enqueue(cook);
				num_wait_vip--;
				num_Cooks_normal--;
				num_serv_vip++;
				OperationDone += "N" + to_string(cook->GetID()) + "(" + "V" + to_string(OptrVI->GetID()) + ")   ";
			}
			else if(!CooksInBreak.isEmpty())
			{
				UrgentVIPOrdersQueue.dequeue(OptrVI);
				CooksInBreak.dequeue(cook);
				OptrVI->setStatus(SRV);
				cook->setSpeed(ceil(float(cook->getSpeed()) / 2.0));
				cook->SetUrgent(true);
				cook->SetOrder(OptrVI, CurrentTimeStep);
				InService.enqueue(OptrVI);
				CooksInservace.enqueue(cook);
				if (cook->GetType() == TYPE_VIP)
				{
					num_Cooks_vip--;
					OperationDone += "V" + to_string(cook->GetID()) + "(" + "V" + to_string(OptrVI->GetID()) + ")   ";
				}
				if (cook->GetType() == TYPE_NRM)
				{
					num_Cooks_normal--;
					OperationDone += "N" + to_string(cook->GetID()) + "(" + "V" + to_string(OptrVI->GetID()) + ")   ";
				}
				if (cook->GetType() == TYPE_VGAN)
				{
					num_Cooks_vegan--;
					OperationDone += "G" + to_string(cook->GetID()) + "(" + "V" + to_string(OptrVI->GetID()) + ")   ";
				}
				num_wait_vip--;
				num_serv_vip++;
			}
			else
			{
				break;
			}
		}

		while (!VIPOrdersQueue.isEmpty())
		{
			if (!VipCooksQueue.isEmpty())
			{
				VIPOrdersQueue.dequeue(OptrVI);
				VipCooksQueue.dequeue(cook);
				OptrVI->setStatus(SRV);
				cook->SetOrder(OptrVI, CurrentTimeStep);
				InService.enqueue(OptrVI);
				CooksInservace.enqueue(cook);
				num_wait_vip--;
				num_serv_vip++;
				num_Cooks_vip--;
				OperationDone += "V"+to_string(cook->GetID())+"("+"V"+to_string(OptrVI->GetID()) +")   ";
			}
			else if (!VeganCooksQueue.isEmpty())
			{
				VIPOrdersQueue.dequeue(OptrVI);
				VeganCooksQueue.dequeue(cook);
				OptrVI->setStatus(SRV);
				InService.enqueue(OptrVI);
				cook->SetOrder(OptrVI, CurrentTimeStep);
				CooksInservace.enqueue(cook);
				num_wait_vip--;
				num_serv_vip++;
				num_Cooks_vegan--;
				OperationDone += "G" + to_string(cook->GetID()) + "(" + "V" + to_string(OptrVI->GetID()) + ")   ";
			}
			else if(!NormalCooksQueue.isEmpty())
			{
				VIPOrdersQueue.dequeue(OptrVI);
				NormalCooksQueue.dequeue(cook);
				OptrVI->setStatus(SRV);
				InService.enqueue(OptrVI);
				cook->SetOrder(OptrVI, CurrentTimeStep);
				CooksInservace.enqueue(cook);
				num_wait_vip--;
				num_serv_vip++;
				num_Cooks_normal--;
				OperationDone += "N" + to_string(cook->GetID()) + "(" + "V" + to_string(OptrVI->GetID()) + ")   ";
			}
			else
			{
				break;
			}
		}

		while (!VeganOrdersQueue.isEmpty())
		{
			if (!VeganCooksQueue.isEmpty())
			{
				VeganOrdersQueue.dequeue(OptrVG);
				VeganCooksQueue.dequeue(cook);
				OptrVG->setStatus(SRV);
				InService.enqueue(OptrVG);
				cook->SetOrder(OptrVG, CurrentTimeStep);
				CooksInservace.enqueue(cook);
				num_wait_vegan--;
				num_serv_vegan++;
				num_Cooks_vegan--;
				OperationDone +="G"+ to_string(cook->GetID()) + "(" + "G" + to_string(OptrVG->GetID()) + ")   ";
			}
			else
			{
				break;
			}
		}

		while (!NormalOrdersQueue.isEmpty())
		{
			if (!NormalCooksQueue.isEmpty())
			{
				NormalOrdersQueue.dequeue(OptrN);
				NormalCooksQueue.dequeue(cook);
				OptrN->setStatus(SRV);
				InService.enqueue(OptrN);
				cook->SetOrder(OptrN, CurrentTimeStep);
				CooksInservace.enqueue(cook);
				num_wait_normal--;
				num_serv_normal++;
				num_Cooks_normal--;
				OperationDone += "N" + to_string(cook->GetID()) + "(" + "N" + to_string(OptrN->GetID()) + ")   ";
			}
			else if (!VipCooksQueue.isEmpty())
			{
				NormalOrdersQueue.dequeue(OptrN);
				VipCooksQueue.dequeue(cook);
				OptrN->setStatus(SRV);
				InService.enqueue(OptrN);
				cook->SetOrder(OptrN, CurrentTimeStep);
				CooksInservace.enqueue(cook);
				num_wait_normal--;
				num_serv_normal++;
				num_Cooks_vip--;
				OperationDone += "N" + to_string(cook->GetID()) + "(" + "G" + to_string(OptrN->GetID()) + ")   ";
			}
			else
			{
				break;
			}
		}


		itoa(num_wait_normal, numwaitingnrm, 10);
		string display1(numwaitingnrm);
		itoa(num_wait_vegan, numwatingvgn, 10);
		string display2(numwatingvgn);
		itoa(num_wait_vip, numwatingvip, 10);
		string display3(numwatingvip);
		//2-///////////////////////////////////////////////////////////////
		//pGUI->PrintMessage("#Num of wating Normal orders " + display1 + "                #Num of wating Vegan orders " + display2 + "             #Num of wating vip orders " + display3 + "                ,click to continue");
		//pGUI->waitForClick();


		// 2- available cooks
		itoa(num_Cooks_normal, numavailnrmcook, 10);
		string display4(numavailnrmcook);
		itoa(num_Cooks_vegan, numavailvgncook, 10);
		string display5(numavailvgncook);
		itoa(num_Cooks_vip, numavailvipcook, 10);
		string display6(numavailvipcook);
		//pGUI->PrintMessage("#Num of available normal cooks " + display4 + "                #Num of available vegan cooks " + display5 + "                #Num of available vip cooks " + display6 + "        ,click to continue");
		//pGUI->waitForClick();
		
		//Sleep(1000);
		//pGUI->PrintMessage(OperationDone);
		if (x==1)
			pGUI->waitForClick();
		else
			Sleep(1000);
		OperationDone = "Cooks and assigned orders: ";

		FillDrawingList();
		pGUI->UpdateInterface();
		pGUI->ResetDrawingList();
		char totalstring[10];
		pGUI->PrintMessage("Current time step: " + display0 + "    $ #Num of wating Normal orders " + display1 + "                #Num of wating Vegan orders " + display2 + "             #Num of wating vip orders " + display3 + ", click to continue $ #Num of available normal cooks " + display4 + "               #Num of available vegan cooks " + display5 + "               #Num of available vip cooks " + display6 + ", click to continue $ " + OperationDone + "$" + "Total Finished Orders " + itoa(total, totalstring, 10) + "$");
		CurrentTimeStep++;	//advance timestep

	}
	
	pGUI->PrintMessage("Click to end the simulation");
	pGUI->waitForClick();

}

void Restaurant::Finalize()
{
	ReadData();
	Event* pEv;
	Cook* cook;
	Order* OptrN;
	Order* OptrVI;
	Order* OptrVG;
	Order* Optr;
	Queue <Order*>TempQueue;
	Queue <Cook*>TempQueueCooks;
	int CurrentTimeStep = 1;
	int num_serv_normal = 0;
	int num_serv_vegan = 0;
	int num_serv_vip = 0;
	int num_Cooks_normal = NumNormCooks;
	int	num_Cooks_vegan = NumVeganCooks;
	int	num_Cooks_vip = NumVipCooks;
	int total = 0;
	int num_injured = 0;
	int num_urgent = 0;
	float auto_promoted = 0;
	while (!EventsQueue.isEmpty() || !InService.isEmpty() || !NormalOrdersQueue.isEmpty() || !VeganOrdersQueue.isEmpty() ||!VIPOrdersQueue.isEmpty()||!UrgentVIPOrdersQueue.isEmpty() )
	{
		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step
		while (!NormalOrdersQueue.isEmpty())
		{
			NormalOrdersQueue.dequeue(Optr);
			if (CurrentTimeStep - Optr->getTime() >= AutoPor)
			{
				Optr->SetOrderType(TYPE_VIP);
				VIPOrdersQueue.enqueue(Optr);
				auto_promoted++;
				num_wait_vip++;
				num_wait_normal--;
			}
			else
				TempQueue.enqueue(Optr);
		}
		while (!TempQueue.isEmpty())
		{
			TempQueue.dequeue(Optr);
			NormalOrdersQueue.enqueue(Optr);
		}
		srand((unsigned)time(0));
		float randNum = (float)rand() / RAND_MAX;
		if (randNum <= InjProp)
		{
			if (!CooksInservace.isEmpty())
			{
				InService.dequeue(Optr);
				CooksInservace.dequeue(cook);
				cook->setIsInj(true);
				cook->setSpeed(ceil(float(cook->getSpeed()) / 2.0));
				cook->SetOrder(Optr, CurrentTimeStep);
				InService.enqueue(Optr);
				num_injured++;
			}
		}
		while (!CooksInBreak.isEmpty())
		{
			CooksInBreak.dequeue(cook);
			if (!cook->IsInBreak())
			{
				if (cook->isUrgent())
				{
					cook->setSpeed(cook->getSpeed() * 2);
					cook->SetUrgent(false);
				}
				if (cook->isInjured())
				{
					cook->setSpeed(cook->getSpeed() * 2);
					cook->setIsInj(false);
				}
				if (cook->GetType() == TYPE_VIP)
				{
					num_Cooks_vip++;
					VipCooksQueue.enqueue(cook);
				}
				else if (cook->GetType() == TYPE_NRM)
				{
					num_Cooks_normal++;
					NormalCooksQueue.enqueue(cook);
				}
				else if (cook->GetType() == TYPE_VGAN)
				{
					num_Cooks_vegan++;
					VeganCooksQueue.enqueue(cook);
				}
			}
			else
				TempQueueCooks.enqueue(cook);
		}
		while (!TempQueueCooks.isEmpty())
		{
			TempQueueCooks.dequeue(cook);
			CooksInBreak.enqueue(cook);
		}
		while (!CooksInservace.isEmpty())
		{
			CooksInservace.dequeue(cook);
			if (!cook->Isbusy())
			{
				if (cook->getOrdersBeforeBreak() == 0)
				{
					srand((unsigned)time(0));
					if (cook->GetType() == TYPE_VIP)
						cook->SetBreak(VipBreakMin + (rand() % (VipBreakMax + 1 - VipBreakMin)));
					if (cook->GetType() == TYPE_NRM)
						cook->SetBreak(NormBerakMin + (rand() % (NormBerakMax + 1 - NormBerakMin)));
					if (cook->GetType() == TYPE_VGAN)
						cook->SetBreak(VegaBreakMin + (rand() % (VegaBreakMax + 1 - VegaBreakMin)));
					CooksInBreak.enqueue(cook);
				}
				else
				{
					if (cook->GetType() == TYPE_VIP)
					{
						num_Cooks_vip++;
						VipCooksQueue.enqueue(cook);
					}
					else if (cook->GetType() == TYPE_NRM)
					{
						num_Cooks_normal++;
						NormalCooksQueue.enqueue(cook);
					}
					else if (cook->GetType() == TYPE_VGAN)
					{
						num_Cooks_vegan++;
						VeganCooksQueue.enqueue(cook);
					}
				}
			}
			else
				TempQueueCooks.enqueue(cook);
		}
		while (!TempQueueCooks.isEmpty())
		{
			TempQueueCooks.dequeue(cook);
			CooksInservace.enqueue(cook);
		}
		while (!InService.isEmpty())
		{
			InService.dequeue(Optr);
			if (Optr->getFinishTime() <= CurrentTimeStep)
			{
				if (Optr->GetType() == TYPE_VIP)
					num_serv_vip--;
				else if (Optr->GetType() == TYPE_NRM)
					num_serv_normal--;
				else
					num_serv_vegan--;
				Optr->setStatus(DONE);
				Finished.enqueue(Optr);
				total++;
			}
			else
				TempQueue.enqueue(Optr);
		}
		while (!TempQueue.isEmpty())
		{
			TempQueue.dequeue(Optr);
			InService.enqueue(Optr);
		}
		while (!VIPOrdersQueue.isEmpty())
		{
			VIPOrdersQueue.dequeue(Optr);
			if (Optr->GetUrgentTimeStep() <= CurrentTimeStep)
			{
				UrgentVIPOrdersQueue.enqueue(Optr);
				num_urgent++;
			}
				
			else
				TempQueue.enqueue(Optr);
		}
		while (!TempQueue.isEmpty())
		{
			TempQueue.dequeue(Optr);
			VIPOrdersQueue.enqueue(Optr);
		}
		while (!UrgentVIPOrdersQueue.isEmpty())
		{
			if (!VipCooksQueue.isEmpty())
			{
				UrgentVIPOrdersQueue.dequeue(OptrVI);
				VipCooksQueue.dequeue(cook);
				OptrVI->setStatus(SRV);
				cook->SetOrder(OptrVI, CurrentTimeStep);
				InService.enqueue(OptrVI);
				CooksInservace.enqueue(cook);
				num_wait_vip--;
				num_serv_vip++;
				num_Cooks_vip--;
			}
			else if (!VeganCooksQueue.isEmpty())
			{
				UrgentVIPOrdersQueue.dequeue(OptrVI);
				VeganCooksQueue.dequeue(cook);
				OptrVI->setStatus(SRV);
				InService.enqueue(OptrVI);
				cook->SetOrder(OptrVI, CurrentTimeStep);
				CooksInservace.enqueue(cook);
				num_wait_vip--;
				num_Cooks_vegan--;
				num_serv_vip++;
			}
			else if (!NormalCooksQueue.isEmpty())
			{
				UrgentVIPOrdersQueue.dequeue(OptrVI);
				NormalCooksQueue.dequeue(cook);
				OptrVI->setStatus(SRV);
				InService.enqueue(OptrVI);
				cook->SetOrder(OptrVI, CurrentTimeStep);
				CooksInservace.enqueue(cook);
				num_wait_vip--;
				num_Cooks_normal--;
				num_serv_vip++;
			}
			else if (!CooksInBreak.isEmpty())
			{
				UrgentVIPOrdersQueue.dequeue(OptrVI);
				CooksInBreak.dequeue(cook);
				OptrVI->setStatus(SRV);
				cook->setSpeed(ceil(float(cook->getSpeed()) / 2.0));
				cook->SetUrgent(true);
				cook->SetOrder(OptrVI, CurrentTimeStep);
				InService.enqueue(OptrVI);
				CooksInservace.enqueue(cook);
				if (cook->GetType() == TYPE_VIP)
					num_Cooks_vip--;
				if (cook->GetType() == TYPE_NRM)
					num_Cooks_normal--;
				if (cook->GetType() == TYPE_VGAN)
					num_Cooks_vegan--;
				num_wait_vip--;
				num_serv_vip++;
			}
			else
				break;
		}
		while (!VIPOrdersQueue.isEmpty())
		{
			if (!VipCooksQueue.isEmpty())
			{
				VIPOrdersQueue.dequeue(OptrVI);
				VipCooksQueue.dequeue(cook);
				OptrVI->setStatus(SRV);
				cook->SetOrder(OptrVI, CurrentTimeStep);
				InService.enqueue(OptrVI);
				CooksInservace.enqueue(cook);
				num_wait_vip--;
				num_serv_vip++;
				num_Cooks_vip--;
			}
			else if (!VeganCooksQueue.isEmpty())
			{
				VIPOrdersQueue.dequeue(OptrVI);
				VeganCooksQueue.dequeue(cook);
				OptrVI->setStatus(SRV);
				InService.enqueue(OptrVI);
				cook->SetOrder(OptrVI, CurrentTimeStep);
				CooksInservace.enqueue(cook);
				num_wait_vip--;
				num_serv_vip++;
				num_Cooks_vegan--;
			}
			else if (!NormalCooksQueue.isEmpty())
			{
				VIPOrdersQueue.dequeue(OptrVI);
				NormalCooksQueue.dequeue(cook);
				OptrVI->setStatus(SRV);
				InService.enqueue(OptrVI);
				cook->SetOrder(OptrVI, CurrentTimeStep);
				CooksInservace.enqueue(cook);
				num_wait_vip--;
				num_serv_vip++;
				num_Cooks_normal--;
			}
			else
				break;
		}
		while (!VeganOrdersQueue.isEmpty())
		{
			if (!VeganCooksQueue.isEmpty())
			{
				VeganOrdersQueue.dequeue(OptrVG);
				VeganCooksQueue.dequeue(cook);
				OptrVG->setStatus(SRV);
				InService.enqueue(OptrVG);
				cook->SetOrder(OptrVG, CurrentTimeStep);
				CooksInservace.enqueue(cook);
				num_wait_vegan--;
				num_serv_vegan++;
				num_Cooks_vegan--;
			}
			else
				break;
		}
		while (!NormalOrdersQueue.isEmpty())
		{
			if (!NormalCooksQueue.isEmpty())
			{
				NormalOrdersQueue.dequeue(OptrN);
				NormalCooksQueue.dequeue(cook);
				OptrN->setStatus(SRV);
				InService.enqueue(OptrN);
				cook->SetOrder(OptrN, CurrentTimeStep);
				CooksInservace.enqueue(cook);
				num_wait_normal--;
				num_serv_normal++;
				num_Cooks_normal--;
			}
			else if (!VipCooksQueue.isEmpty())
			{
				NormalOrdersQueue.dequeue(OptrN);
				VipCooksQueue.dequeue(cook);
				OptrN->setStatus(SRV);
				InService.enqueue(OptrN);
				cook->SetOrder(OptrN, CurrentTimeStep);
				CooksInservace.enqueue(cook);
				num_wait_normal--;
				num_serv_normal++;
				num_Cooks_vip--;
			}
			else
				break;
		}
		CurrentTimeStep++;	
	}
	int total_wait = 0;
	int total_serv = 0;
	Order* order;
	ofstream cout;
	cout.open("output.txt");
	cout << left<< setw(8)<<"FT"<< setw(8)<<"ID"<< setw(8)<<"AT"<< setw(8)<<"WT"<< setw(8)<<"ST" << endl;
	while (!Finished.isEmpty())
	{
		Finished.dequeue(order);
		total_serv = total_serv + order->getServTime();
		total_wait = total_wait + (order->getFinishTime() - order->getServTime() - order->getTime());
		cout <<left<<setw(8)<< order->getFinishTime() << setw(8) << order->GetID() << setw(8) << order->getTime() << setw(8) << order->getFinishTime()-order->getServTime() - order->getTime() << setw(8) <<  order->getServTime() << endl;
	}
	float n =num_veg_orders+num_vip_orders+num_norm_orders ;
	cout << "Orders : " << n << "   [ Norm : " << num_norm_orders << ", Veg : "<< num_veg_orders << ", VIP :"<< num_vip_orders << " ]" << endl;
	//cout<<"Cooks : "<<num_Cooks_normal+num_Cooks_vegan+num_Cooks_vip<< "   [ Norm : " << num_Cooks_normal << ", Veg : " << num_Cooks_vegan << ", VIP :" << num_Cooks_vip <<", Injured : "<<num_injured <<" ]" << endl;
	cout << "Cooks : " << NumNormCooks+NumVeganCooks+NumVipCooks << "   [ Norm : " << NumNormCooks << ", Veg : " << NumVeganCooks << ", VIP :" << NumVipCooks << ", Injured : " << num_injured << " ]" << endl;
	cout << "Avg wait = " << total_wait / n << " ,    Avg serv = " << total_serv / n << endl;
	cout << "Urgent Orders = " << num_urgent << " ,    Auto promoted  "<< (auto_promoted /num_norm_orders)*100 <<" %"<<endl;
}

