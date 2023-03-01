#pragma once
#include "Event.h"
class Promotion :public Event
{
    int extraMoney; //extra money needed for promotion
    
public:
    Promotion(Time event, int id,int extra); //makes needed initializations
    bool execute(Cargo*& ptr, Company* pComp)const;          //overrides execute of event (NOT FINISHED YET)
   

};

