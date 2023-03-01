#include "Truck.h"

Truck::Truck(Trucks type, int capacity, int checkup, int S, int id, bool shift)
{   //make needed initializations
	cargoNum = 0;
	deliveryDistance = 0;
	J = 0;
	loadTime = -1;
	checkupStartTime.Setday(-1);
	checkupStartTime.Sethour(-1);
	allDeliveries = 0;
	NightShift = shift;
	truckFail = 0;
	setCheckup(checkup);
	setSpeed(S);
	setTC(capacity);
	setType(type);
	setId(id);
	truckCargo.initialize(capacity);
	activeTime = 0;
}

void Truck::setType(Trucks Type)
{
	type = Type;
}

void Truck::setTC(int capacity)
{
	TC = capacity;
}

void Truck::setCheckup(int checkup)
{
	maintenanceTime = checkup;
}

void Truck::setSpeed(int S)
{
	Speed = S;
}

void Truck::setInterval(int Interval)
{
	DI = Interval;
}

void Truck::setId(int id)
{
	TID = id;
}

void Truck::setMovingTime(Time moving)
{
	movingTime = moving;
}

void Truck::setEndingHours(Time ending)
{
	endingTime = ending;
}

Time Truck::getEndingTime()
{
	return endingTime;
}

void Truck::incrementAllDeliveries(int n)
{
	allDeliveries += n;
}

int Truck::getAlldeliveries() const
{
	return allDeliveries;
}

int Truck::getCapacity() const
{
	return TC;
}

int Truck::getCargoNum() const
{
	return cargoNum;
}

void Truck::addCargo(Cargo* pCargo)
{
	cargoNum++;
	truckCargo.enqueue(pCargo);
}

void Truck::printCargo() const
{
	truckCargo.print();
}

int Truck::getID() const
{
	return TID;
}

int Truck::getSpeed() const
{
	return Speed;
}

int Truck::getJourney()
{
	return J;
}

void Truck::incrementJ()
{
	J++;
}

void Truck::setJ(int j)
{
	J = j;
}

Time Truck::getstartingtime()
{
	return startTime;
}

void Truck::setStatringtime(Time start)
{
	startTime = start;
}

void Truck::setdeliverDisatnce(int d)
{
	deliveryDistance = d;
}

int Truck::getDeliveryDistance() const
{
	return deliveryDistance;
}

Cargo* Truck::getFrontCargo()
{
	return truckCargo.front();
}

Cargo* Truck::removeCargo()
{
	cargoNum--;
	return truckCargo.dequeue();
}

Time Truck::getMovingTime() const
{
	return movingTime;
}

bool Truck::getNightShift() const
{
	return NightShift;
}



Trucks Truck::getType()
{
	return type;
}



void Truck::setStartCheckUp(Time startCheckUp)
{
	checkupStartTime = startCheckUp;
}

int Truck::getEndCheckupTime()
{
	return checkupStartTime.getDay() * 24 + checkupStartTime.getHour() + maintenanceTime;
}

int Truck::getActiveTime()
{
	return activeTime;
}

void Truck::incrementActiveTime(int active)
{
	activeTime += active;
}

bool Truck::operator>(Truck other)
{
	if ((this->getSpeed() * 0.5 - this->getCapacity() * 5) < (other.getSpeed() * 0.5 - other.getCapacity() * 5))
		return 1;
	else
		return 0;
}

bool Truck::operator<(Truck other)
{
	if ((this->getSpeed() * 0.5 - this->getCapacity() * 5) > (other.getSpeed() * 0.5 - other.getCapacity() * 5))
		return 1;
	else
		return 0;
}

void Truck::setTruckFailure()
{
	truckFail = rand() % 100 + 1;
}

int Truck::getTruckFailure()
{
	return truckFail;
}

void Truck::setLoadTime(int hours)
{
	loadTime = hours;
}

int Truck::getLoadtTime() const
{
	return loadTime;
}

bool Truck::decrenmetntLoading()
{
	bool check = 0;
	loadTime--;
	if (loadTime == 0)
		check = 1;
	else
		check = 0;

	return check;
}