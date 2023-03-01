#pragma once
#include"Time.h"
#include "Cargo.h"
#include <iostream>
class Company;

using namespace std;
class Event //base class for Preparation, Cancellation, and Promotion events
{
	Time eventTime; //event time
	int id;         //related cargo info 
public:
	Event(Time event,int ID);					 //make needed initializations
	virtual bool execute(Cargo *& ptr,Company*pComp) const= 0; //will be overriden in Preparation, Cancellation, and Promotion events
	Time getTime()const;					     //return Time as an object giving us hour and day
	int getID()const;							 //return ID

};


