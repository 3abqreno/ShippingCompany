#include "Promotion.h"
#include"Company.h"
Promotion::Promotion(Time event, int id, int extra):Event(event,id)
{
	extraMoney = extra;
}

bool Promotion::execute(Cargo*& ptr, Company* pComp) const

{
	return pComp->promote(getID(), extraMoney);
	
}
