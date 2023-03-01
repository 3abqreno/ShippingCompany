#pragma once
#include<iostream>
#include<string>
#include"Cargo.h"
#include"Event.h"
#include"Promotion.h"
#include"Cancel.h"
#include"Ready.h"
#include <chrono>
#include <thread>
#include"Time.h"
#include"pQueue.h"
#include"Queue.h"
#include"Linked List.h"
#include"Truck.h"
using namespace std;

class UI
{

public:
	UI();

	int ChooseMode();	         //take mode input from user
	void printCurrentTime(Time simulatiomTime);     //prints current time
	void printHyphens();         //prints hyphens as it's repeated in code
	void Sleep();				 //pauses for 1 second in stepbystepmode
	void SilentModeOutput();     //outputs to console silent mode
	void interactiveCondition(); //makes user interact with console
	void printEndl();
	//print waiting
	void printWaitingCargos(int w);
	void printVipWaitingCargos(pQueue <Cargo*>& waitingQueue);
	void printSpecialWaitingCargos(Queue <Cargo*> waitingQueue);
	void printNormalWaitingCargos(LinkedList <Cargo*> waitingQueue);
	//print loading
	void printLoadingTrucks(int l);
	void printVipLoadingTrucks(Truck* pTruck);
	void printSpecialLoadingTrucks(Truck* pTruck);
	void printNormalLoadingTrucks(Truck* pTruck);
	//print empty
	void printEmptyTrucksCount(int e);
	void printEmptyTrucks(pQueue <Truck*>& vip, pQueue <Truck*>& special, pQueue <Truck*>& normal);
	void printVipEmptyTrucks(pQueue <Truck*>& emptyTrucks);
	void printSpecialEmptyTrucks(pQueue <Truck*>& emptyTrucks);
	void printNormalEmptyTrucks(pQueue <Truck*>& emptyTrucks);
	//print moving
	void printMovingCargos(int m);
	void printVipMovingCargos(Queue <Truck*>& movingTrucks);
	void printSpecialMovingCargos(Queue <Truck*>& movingTrucks);
	void printNormalMovingCargos(Queue <Truck*>& movingTrucks);
	//print incheckup
	void printIncheckupTrucks(int c);
	void printVipIncheckupTrucks(Queue <Truck*>& checkupTrucks);
	void printSpecialIncheckupTrucks(Queue <Truck*>& checkupTrucks);
	void printNormalIncheckupTrucks(Queue <Truck*>& checkupTrucks);
	//print delivered
	void printDeliveredCargos(int d);
	void printDeliveredCargoQueues(Queue <Cargo*>& deliveredCargos);
	//chose input and output filenames
	string chooseFilename();
	string chooseInputFilename();
};



