#include "Cancel.h"
#include"Company.h"
Cancel::Cancel(Time event, int id):Event(event,id)
{

}

bool Cancel::execute(Cargo*& ptr, Company* pComp)const
{
	return pComp->cancel(getID());

}
