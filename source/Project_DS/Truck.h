#pragma once
#include"Cargo.h"
#include"Queue.h"
#include"Time.h"
#include"pQueue2.h"
enum  Trucks { vipTruck, specialTruck, normalTruck };
class Truck
{
	Trucks type;				 //(VIP/NORMAL/SPECIAL)
	int TC;						 //Truck Capacity (read from input file)
	int cargoNum;				 //??
	int maintenanceTime;		 //duration in hours for checkup after J amount of journeys read from input file
	int Speed;					 //kilometers/hour
	int DI;						 //Delivery Interval 
	int TID;					 //the id of the truck assumed by us
	Time movingTime;		     //the time in which the truck started moving
	Time startTime;				 //time that the truck starts loading
	int deliveryDistance;		 //farthest cargo to be delivered
	int J;						 //current number of journeys before maintenance
	Time checkupStartTime;		 //time at which it started checkup
	Time endingTime;
	pQueue2 <Cargo*> truckCargo; //priority queue of cargos in truck sorted by CDT
	Time endTime;				 //time at which truck drops last cargo
	int activeTime;              //active time for truck which is the difference between start time and end time
	int loadTime;				 //
	int allDeliveries;			 //
	bool NightShift;			 //indicator that truck can work at night shift
	int truckFail;               //will have a number between 1->100 (if this number is 34 the 
public:
	Truck(Trucks type, int capacity, int checkup, int S, int id, bool shift);

	void setTC(int capacity);				 //set Truck Capacity from input file
	void setCheckup(int checkup);			 //set maintenanceTime
	void setSpeed(int S);					 //set Speed in kilometers/hour
	void setType(Trucks Type);				 //set (VIP/NORMAL/SPECIAL)
	void setInterval(int Interval);			 //set delivery interval
	void setId(int id);						 //set TID
	void setMovingTime(Time moving);		 //set moving time of truck
	void setEndingHours(Time ending);
	Time getEndingTime();
	int getCargoNum()const;					 //??
	int getCapacity()const;					 //get TC of truck
	void addCargo(Cargo* pCargo);			 //addCargo to truckCargo priority queue
	void printCargo()const;					 //print all Cargo IDs in the truck
	int getID()const;						 //get TID
	int getSpeed()const;					 //get truck speed
	int getJourney();						 //get current journey before maintenance
	void incrementJ();						 //increment number of journeys done by the truck
	void setJ(int j);						 //set current journey before maintenance
	Time getstartingtime();					 //get starting time
	Trucks getType();                        //returns type
	void setStatringtime(Time start);		 //set starting time
	void setdeliverDisatnce(int d);			 //set delivery distance
	int getDeliveryDistance()const;			 //get delivey distance
	Cargo* getFrontCargo();					 //get the cargo at front of truck 
	Cargo* removeCargo();					 //remove cargo from truck according to its priority
	Time getMovingTime()const;				 //get moving time of the truck
	bool getNightShift()const;				 //get night shift boolean
	void setStartCheckUp(Time startCheckUp); //set start checkup time of the truck
	int getEndCheckupTime();				 //get the ending of the checkup time of the truck
	int getActiveTime();					 //return active time of the truck
	void incrementActiveTime(int active);
	bool operator >(Truck other);
	bool operator <(Truck other);
	void setTruckFailure();					//set truck failure required in bonus
	int getTruckFailure();					//get truck failure rquired in bonus
	void setLoadTime(int hours);			//set load time
	int getLoadtTime()const;				//get load time
	bool decrenmetntLoading();				//decreament loading
	void incrementAllDeliveries(int n);		//increment deliveried to this truck with passed number
	int getAlldeliveries()const;			//return all deliveries
};

