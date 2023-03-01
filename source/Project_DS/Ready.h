#pragma once
#include "Event.h"
#include"Cargo.h"
class Ready : public Event
{
	//WHY NOT IN EVENT BASE CLASS
	int loadTime;
	cargos cargoType;
	int deliveryDistance;
	int cost;
public:
	Ready(Time event,int id,int load, char type,int DD,int price); //makes needed initializations
	bool execute(Cargo*& ptr, Company* pComp) const;							   //overrides execute of event (NOT FINISHED YET)
	void printdata() const;										   //prints data of event (WHY)

};

