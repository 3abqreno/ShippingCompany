#pragma once
enum cargos { vipCargo, specialCargos, normalCargos };
#include<iostream>
#include "Time.h"
using namespace std;
class Cargo
{
	Time PT;			 //Preperation Time (ready to be assigned to a truck)
	int id;				 //ID
	int loadTime;	     //time in hours to load/unload the cargo to/from the truck
	cargos cargoType;	 //(VIP/NORMAL/SPEICIAL)
	int deliveryDistance;//distance in kilometers from the company to the delivery location of the cargo
	int cost;			 //cost of delivering cargo
	Time CDT;			 //Cargo Delivery Time, MT + CargoDistance/TruckSpeed + Cargo unload Time 
	int waitingTime;     //WT = MT- PT
	int truckID;         //ID of truck the cargo is delivered in
	int promoted;        //promoted or not (0->yes,1->no)
	
public:
	Cargo(Time prep, int id, int load, cargos type, int dist, int price);
	void setID(int ID);					    //sets ID
	void setLT(int load);					//set Cargo LoadTime
	void setType(cargos type);				//set (VIP/NORMAL/SPEICIAL)
	void setDD(int DD);						//set delivery distance
	void setCost(int price);				//set cargo cost
	void setCDT(Time cdt);					//set cargo delivery time
	Time getCDT()const;						//get cargo delivery time
	int getDD()const;					    //get delivery distance
	void incrementCargo(int extra);			//used for promotion event
	cargos getType();						//get (VIP/NORMAL/SPEICIAL)
	int getID()const;						//get ID
	float getPriority();					//get priority
	Time getPreperationTime()const;			//get PT
	void setTruckID(int id);				//set ID of truck the cargo is in
	int getTruckID()const;					//get ID of truck the cargo is in
	int getLoadTime() ;						//get loadtime
	int getWaitingTime();					//get waitingtime
	void incrementPromoted();				//set boolean of the promoted count with 1
	int getPromoted();						//get promoted boolean
	void setWaitingTime(int hours);         //set waiting time of cargo 
	bool operator <=(Cargo& other);			//operator overloading
	bool operator>=(Cargo& other);			//operator overloading
	bool operator >(Cargo& other);			//operator overloading
	bool operator <(Cargo& other);			//operator overloading
	bool operator==(int ID);				//operator overloading
	friend ostream& operator << (ostream& out, const Cargo &C);
};
