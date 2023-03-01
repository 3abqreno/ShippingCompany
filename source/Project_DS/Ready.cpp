#include "Ready.h"

Ready::Ready(Time event,int id,int load, char type, int DD, int price):Event(event,id)
{
	loadTime = load;
	deliveryDistance = DD;
	cost = price;
	if (type == 'N')
		cargoType = normalCargos;
	else if (type == 'V')
		cargoType = vipCargo;
	else
		cargoType = specialCargos;
}

bool Ready::execute(Cargo *& ptr, Company* pComp)const
{
	ptr = new Cargo(getTime(),getID(),loadTime,cargoType,deliveryDistance,cost);
	return true;
}

void Ready::printdata() const
{
	cout << loadTime << endl;
	cout << getID() << endl;
	cout << deliveryDistance << endl;
	cout << getTime().getDay() << " " << getTime().getHour()<<endl;
	cout << cargoType<<endl;
	cout << cost;

}
