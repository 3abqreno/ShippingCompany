#include "Cargo.h"
ostream& operator << (ostream& out, const Cargo& C)
{
	out << C.getID();
	return out;
}
Cargo::Cargo(Time prep, int id, int load, cargos type, int dist, int price) :PT(prep)
{   //make all needed initializations
	setID(id);
	setLT(load);
	setType(type);
	setDD(dist);
	setCost(price);
	CDT.Setday(-1);
	CDT.Sethour(-1);
	promoted = 0;
	truckID = -1;
}

void Cargo::setID(int ID)
{
	id = ID;
}

void Cargo::setType(cargos type)
{
	cargoType = type;
}

void Cargo::setLT(int load)
{
	loadTime = load;
}



void Cargo::setDD(int DD)
{
	deliveryDistance = DD;
}

void Cargo::setCost(int price)
{
	cost = price;
}

void Cargo::setCDT(Time cdt)
{
	CDT = cdt;
}

Time Cargo::getCDT() const
{
	return CDT;
}

int Cargo::getDD() const
{
	return deliveryDistance;
}

int Cargo::getLoadTime() 
{
	return loadTime;
}

void Cargo::incrementCargo(int extra)
{
	cost += extra;
}

cargos Cargo::getType()
{
	return cargoType;
}

int Cargo::getID()const
{
	return id;
}

void Cargo::setWaitingTime(int hours)
{
	waitingTime = hours;
}

float Cargo::getPriority()
{
	if (cargoType != vipCargo)
		return 0;

	return (((24 * PT.getDay() + PT.getHour()) * deliveryDistance / (float)cost));
}
Time Cargo::getPreperationTime() const
{
	return PT;
}
void Cargo::setTruckID(int id) 
{
	truckID = id;
}

int Cargo::getWaitingTime()
{
	return waitingTime;
}

void Cargo::incrementPromoted()
{
	promoted = 1;
}

int Cargo::getPromoted()
{
	return promoted;
}

int Cargo::getTruckID() const
{
	return truckID;
}
bool Cargo::operator>=(Cargo& other)
{
	if (this->deliveryDistance >other.deliveryDistance )
		return 1;
	else
		return 0;
}

bool Cargo::operator<=(Cargo& other)
{
	if (this->deliveryDistance<other.deliveryDistance)
		return 1;
	else
		return 0;
}

bool Cargo::operator>(Cargo& other)
{
	if (other.getPriority() < this->getPriority())
		return 1;
	else
		return 0;
}

bool Cargo::operator<(Cargo& other)
{
	if (other.getPriority() > this->getPriority())
		return 1;
	else
		return 0;
}

bool Cargo::operator==(int ID)
{
	if (id == ID)
		return true;
	else
		return false;
}
