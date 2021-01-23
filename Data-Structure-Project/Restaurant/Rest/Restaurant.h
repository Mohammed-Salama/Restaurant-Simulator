#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include "..\Events\Event.h"
#include "..\Events\PromotionEvent.h"
#include "..\Events\CancelationEvent.h"

#include<fstream>
#include "Order.h"
#include "..\Generic_DS\Priority_Queue.h"

bool VIPWeightFunc(Order* a, Order* b);
bool FinishedFunc(Order* a, Order* b);
// it is the maestro of the project
class Restaurant  
{	
private:
	GUI *pGUI;
	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file

	
	/// ==> 
	//	DEMO-related members. Should be removed in phases 1&2
	Queue<Order*> DEMO_Queue;	//Important: This is just for demo
	/// ==>
	Queue<Order*> VeganOrdersQueue;
	Priority_Queue<Order*, VIPWeightFunc> UrgentVIPOrdersQueue;
	Queue<Order*> NormalOrdersQueue;
	Priority_Queue<Order*,VIPWeightFunc> VIPOrdersQueue;
	Queue<Cook*> VipCooksQueue;
	Queue<Cook*> NormalCooksQueue;
	Queue<Cook*> VeganCooksQueue;
	Queue<Cook*> CooksInservace;
	Queue<Cook*> CooksInBreak;
	Queue<Order*> InService;

	////////////////////////////////////////////////////////////////////////////////attention please ////////////////////////
	// these queues must be replaced by Priority queues in phase2
	Priority_Queue<Order*, FinishedFunc> Finished;
	// these queues must be replaced by Priority queues in phase2
	////////////////////////////////////////////////////////////////////////////////attention please ////////////////////////

	
	//
	// TODO: Add More Data Members As Needed
	int NumVipCooks;//Number of Vip cooks
	int NumNormCooks;//Number of Normal cooks
	int NumVeganCooks;//Number of Veagan cooks
	int VipSpeedMin;//How Many Could the Vip CooK Do in one Time Step
	int VipSpeedMax;//How Many Could the Vip CooK Do in one Time Step
	int NormSpeedMin;//How Many Could the Normal CooK Do in one Time Step
	int NormSpeedMax;//How Many Could the Normal CooK Do in one Time Step
	int VeganSpeedMin;//How Many Could the Vegan CooK Do in one Time Step
	int VeganSpeedMax;//How Many Could the Vegan CooK Do in one Time Step
	int NumBeforeBreak;//Nunmber Of Orders Before Break
	int VipBreakMin;//Vip Break Duration
	int VipBreakMax;//Vip Break Duration
	int NormBerakMin;//Normal Break Duration
	int NormBerakMax;//Normal Break Duration
	int VegaBreakMin;//Vegan Break Duration
	int VegaBreakMax;//Vegan Break Duration
	int AutoPor;//number of timesteps to automaticly convert Normla to VIP
	int NumEvents;//Number Of Events
	float InjProp;//The probability a busy cook gets injured
	int RstPrd;//Rest period of the cook that got injured
	int VIP_WT;//The number of ticks after which a VIP order is considered urgent
	int num_veg_orders;
	int num_vip_orders;
	int num_norm_orders;
	//

	int num_wait_normal;
	int num_wait_vegan;
	int num_wait_vip;

	int mts;

public:
	
	Restaurant();
	~Restaurant();
	
	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation();
	//Queue<Order*>* getVeganOrders();
	//Queue<Order*>* getNormalOrders();
	//Queue<Order*>* getVIPOrders();
	//void setVeganOrders(Queue<Order*> Qu);
	//void setNormalOrders(Queue<Order*> Qu);
	//void setVIPOrders(Queue<Order*> Qu);
	void FillDrawingList();
	//
	// TODO: Add More Member Functions As Needed
	void ReadData();


	void Simulate(int x);
	//void Simulate1();


	void Finalize();
	//


/// ===================    DEMO-related functions. Should be removed in phases 1&2   ================= 

	//void Just_A_Demo();	//just to show a demo and should be removed in phase1 1 & 2
	void AddtoDemoQueue(Order* po);	//adds an order to the demo queue

/// ================================================================================================== 

	bool AddtoSuitableQueue(Order*);
	Order* RemovefromSuitableQueue(Order*);
	Order* RemovefromSuitableQueue(int , ORD_TYPE);
};


#endif

