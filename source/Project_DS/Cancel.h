#pragma once
#include "Event.h"
class Cancel : public Event
{


public:
    Cancel(Time event, int id);
    bool execute(Cargo*& ptr, Company* pComp) const; //overrides execute of event (NOT FINISHED YET)

};

