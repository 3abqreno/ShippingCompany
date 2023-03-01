#pragma once
#include"Queue.h"
#include<fstream>
#include"Truck.h"
#include"Linked List.h"
#include"pQueue.h"
#include "UI.h"
using namespace std;
enum mode { interactiveMode, stepbystepMode, silentMode }; //user chooses which mode using this enum

class Event;

class Company
{
	//data members
	int numNormal, numSpecial, numVip;      //no. of trucks of each type (N, S, V)
	int speedNormal, speedSpecial, speedVip;//truck speeds of each type (km/h)
	int capNormal, capSpecial, capVIp;      //Capacity of each truck type (N, S, V)
	int numJorneys;							//no. of journeys before checkup 
	int checkNormal, checkSpecial, checkVip;//the checkup durations of normal, special and Vip
	int autoP, maxW;						//auto promotion limit (days)/ max waiting (hours)
	int numEvents;							//number of events in given file
	int movingCargo;						//number of cargo moving in trucks
	//needed lists
	Queue <Event*> eventList;
	//waiting queues
	pQueue <Cargo*> waitingvipCargos;
	Queue <Cargo*> waitingspecialCargos;
	LinkedList <Cargo*> waitingnormalCargos;
	//delivered queue
	Queue<Cargo*>deliveredCargo;
	//Empty truck queues
	pQueue <Truck*> emptyvipTrucks;
	pQueue <Truck*> emptyspecialTrucks;
	pQueue <Truck*> emptynormalTrucks;
	//Loading truck queues
	Truck* loadingvipTruck;
	Truck* loadingspecialTruck;
	Truck* loadingnormalTruck;
	//moving truck queues
	Queue <Truck*> movingvipTrucks;
	Queue <Truck*> movingspecialTrucks;
	Queue <Truck*> movingnormalTrucks;
	//checkup truck queues
	Queue <Truck*> checkupvipTrucks;
	Queue <Truck*> checkupspecialTrucks;
	Queue <Truck*> checkupnormalTrucks;
	//UI pointer
	UI* pUI;
	//simulation data members
	Time simulationTime;
	int activeHours;
	mode modeType;
	//statistics & Output
	Time averageWaitingTime;
	int autopromotedPercentage;
	int averageTrucksActiveTime;
	int averageTrucksUtilizationTime;
	

public:
	Company();
	//functions for reading company data initially from file
	void readParameters(ifstream& myFile);	              //reads all data members from file
	void readEvents(int num, ifstream& myFile);           //reads the events from file knowing that numbers of events = num
	void enqueueEvent(Event* pEvent);		              //puts all events into the eventlist queue
	Ready* readReady(ifstream& myFile);		              //functions used inside readEvents to read readyEvent in switch case
	Cancel* readCancel(ifstream& myFile);	              //functions used inside readEvents to read cancelEvent in switch case
	Promotion* readPromotion(ifstream& myFile);           //functions used inside readEvents to read promotionEvent in switch case
	//simulation									      
	void Simulation();						              //main function of this class that prints and stimulates event
	void setMode(int mode);					              //setter for mode
	//functions for simulation of phase 1			      
	void eventToCargo();					              //puts in cargo lists (NOT FINISHED YET)
	bool checkSimulationworking();				          //the condition which makes simulation stop
	void enqueueCargo(Cargo* pCargo);			          //enqueues normal
	//truck related functions						      
	void makeTrucks(ifstream& myFile);					  //creates new trucks
	void assignCargo();								      //calls load functions
	bool LoadNormalCargo(bool maxW);					  
	bool LoadSpecialCargo(bool maxW);					      
	bool loadVipCargo(bool maxW);							      
	void movingTruckstoDelivery(Queue<Truck*>&qTruck);    
	bool checkMaxW();					                  //Maw weight
	void moveTruckToEmptyOrCheckup(Queue<Truck*>& qTruck);//truck to to empty or checkup according to current number of journeys of truck
	void checkupToEmpty();							      //truck go from checkup to empty
	void loadingToMoving(Truck*& pTruck);			      
	bool decrementLoadTime(Truck* pTruck);				
	Truck* getNightCar(pQueue<Truck*>&qTruck,int count);
	void checkFailure();								  //creates random number and checks if it is failed or not
	//functions for poromation and cancellation 	      
	bool promote(int key, int extraCost);                 //promotion
	bool autopromotion();							      //promoted according to autoP
	bool cancel(int key);							      //cancellation
	//functions to help in debugging
	void printData()const;							  //print all data of company class
	void printEvent();								  //prints all events in the eventlist queue (NOT FINISHED YET)
	int getWaitingCount();							  //get number of waiting cargos
	int getLoadingCount();							  //get number of loading cargos
	int getNormalEmptyCount();						  //get number of normal delivered cargos	
	int getSpecialEmptyCount();						  //get number of special delivered cargos	
	int getVipEmptyCount();						      //get number of vip delivered cargos	
	int getEmptyCount();							  //get number of empty trucks	
	int getMovingCount();							  //get number of moving cargo	
	int getCheckupCount();							  //get number of incheckup trucks	
	int getNormalDeliveredCount();					  //get number of normal delivered cargos	
	int getSpecialDeliveredCount();					  //get number of special delivered cargos	
	int getVipDeliveredCount();						  //get number of vip delivered cargos	
	int getCheckUpCount();
	int getNumberTrucks()const;
	int getDeliveredCount();						  //get number of total delivered cargos	
	void executeEvent();							  //executes 1 event for phase 1 sism
	void printEvents();								  //temp function for diagnosis only
	//Output File
	void outputFile();								  //the output file that will be at the end
	void calcCargoAverageWaitingTime();	              //calculate averageWaitingTime
	void calcAutopromotedPercentage();		          //calculate autopromotedPercentage
	void calcAverageTrucksActiveTime();		          //calculate averageTrucksActiveTime
	void calcAverageTrucksUtilizationTime();          //calculate averageTrucksUtilizationTime
	//destructors

	template <class t>
	void deleteQueue(Queue<t>& x);

	template <class t>
	void deletePQueue(pQueue<t>& x);

	template <class t>
	void deleteLinkedList(LinkedList<t>& x);

	void desructCompany();
};

template <class t>
void Company::deleteQueue(Queue<t>& x)
{
	while (!x.isEmpty())
	{
		t temp = x.dequeue();
		delete temp;
		
	}
}

template <class t>
void Company::deletePQueue(pQueue<t>& x)
{
	while (!x.isEmpty())
	{
		t temp = x.dequeue();
		delete temp;
		
	}
}

template <class t>
void Company::deleteLinkedList(LinkedList<t>& x)
{
	t data;
	while (x.getFront())
	{
		data=x.deleteFront();
		delete data;
	}
}