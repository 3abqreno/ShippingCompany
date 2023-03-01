#include "Company.h"
#include"Event.h"

Company::Company() :waitingvipCargos(numVip) {

	pUI = new UI;
	activeHours = 0;
	movingCargo = 0;
	setMode(pUI->ChooseMode());
	string inputFile = pUI->chooseInputFilename();
	ifstream myFile;
	myFile.open(inputFile);

	readParameters(myFile);
	makeTrucks(myFile);
	waitingvipCargos.initialize(numVip);
	readEvents(numEvents, myFile);

	//statistics
	averageWaitingTime.Setday(-1);
	averageWaitingTime.Sethour(-1);
	autopromotedPercentage = -1;
	averageTrucksActiveTime = -1;
	averageTrucksUtilizationTime = -1;
}

void Company::readParameters(ifstream& myFile)
{
	myFile >> numNormal >> numSpecial >> numVip;
	myFile >> numJorneys >> checkNormal >> checkSpecial >> checkVip >> autoP >> maxW >> numEvents;

}

void Company::readEvents(int num, ifstream& myFile)
{
	Event* pEvent = NULL;
	char type;
	for (int i = 0; i < num; i++)
	{

		myFile >> type;
		switch (type)
		{
		case 'R':
			pEvent = readReady(myFile);
			break;

		case 'X':
			pEvent = readCancel(myFile);
			break;

		case'P':
			pEvent = readPromotion(myFile);
			break;

		default:
			break;
		}
		eventList.enqueue(pEvent);
	}
}

void Company::enqueueEvent(Event* pEvent)
{
	eventList.enqueue(pEvent);
}
void Company::enqueueCargo(Cargo* pCargo)
{
	waitingnormalCargos.insertEnd(pCargo);
}

Ready* Company::readReady(ifstream& myFile) // reads ready events
{

	char type;
	int day, hour;
	int id, distance, loadtime, cost;
	myFile >> type >> day;
	myFile.ignore();
	myFile >> hour >> id >> distance >> loadtime >> cost;
	Time prep(day, hour);
	Ready* ptr = new Ready(prep, id, loadtime, type, distance, cost);
	return ptr;
}

Cancel* Company::readCancel(ifstream& myFile) // reads cancel events
{
	int id;
	int day, hour;
	myFile >> day;
	myFile.ignore();
	myFile >> hour >> id;
	Time prep(day, hour);
	Cancel* ptr = new Cancel(prep, id);
	return ptr;
}

Promotion* Company::readPromotion(ifstream& myFile) // reads promotion events
{
	int id;
	int day, hour;
	myFile >> day;
	myFile.ignore();
	myFile >> hour >> id;
	int extraMoney;
	myFile >> extraMoney;
	Time prep(day, hour);
	Promotion* ptr = new Promotion(prep, id, extraMoney);
	return ptr;

	cout << day << " " << hour << " " << id << " " << extraMoney << endl;
}
void Company::makeTrucks(ifstream& myFile)
{
	emptynormalTrucks.initialize(numNormal);
	emptyspecialTrucks.initialize(numSpecial);
	emptyvipTrucks.initialize(numVip);
	Truck* pTruck = nullptr; \
		int speed, cap, night, id;
	for (int i = 0; i < numNormal; i++)
	{
		myFile >> id >> speed >> cap >> night;
		pTruck = new Truck(normalTruck, cap, checkNormal, speed, id, night);
		emptynormalTrucks.enqueue(pTruck);
	}
	for (int i = 0; i < numSpecial; i++)
	{
		myFile >> id >> speed >> cap >> night;
		pTruck = new Truck(specialTruck, cap, checkNormal, speed, id, night);
		emptyspecialTrucks.enqueue(pTruck);
	}
	for (int i = 0; i < numVip; i++)
	{
		myFile >> id >> speed >> cap >> night;
		pTruck = new Truck(vipTruck, cap, checkNormal, speed, id, night);
		emptyvipTrucks.enqueue(pTruck);
	}


}
void Company::assignCargo()
{
	loadVipCargo(0);
	LoadSpecialCargo(0);
	LoadNormalCargo(0);


}
bool Company::LoadNormalCargo(bool maxW = 0)
{
	//checks for loadiong rule
	if (!waitingnormalCargos.getCount() || loadingnormalTruck || (emptynormalTrucks.isEmpty() && emptyvipTrucks.isEmpty()))return false;
	bool check = false;
	Cargo* pCargo = nullptr;
	Truck* pTruck = nullptr;
	pQueue2<Cargo*>cargoQ;
	int  hours;
	int addedHours = 0;
	int count = waitingnormalCargos.getCount();
	//check for night and day shift
	if (simulationTime.getHour() < 23 && simulationTime.getHour() >= 5) {
		if (!emptynormalTrucks.isEmpty()) {
			pTruck = emptynormalTrucks.front();
		}
		else if (!emptyvipTrucks.isEmpty()) {
			pTruck = emptyvipTrucks.front();
		}
	}
	else {
		//gets a car that can work in night shift
		pTruck = getNightCar(emptynormalTrucks, count);
		if (!pTruck)
			pTruck = getNightCar(emptyvipTrucks, count);
	}
	if (!pTruck)return 0;
	if (pTruck->getCapacity() <= waitingnormalCargos.getCount() || maxW) {
		int movingCount = 0;
		if (maxW) {
			movingCount = waitingnormalCargos.getCount();
		}
		else
		{
			movingCount = pTruck->getCapacity();
		}
		int  loadTime = 0, distance = 0;
		cargoQ.initialize(movingCount);
		loadingnormalTruck = pTruck;
		pTruck->incrementAllDeliveries(movingCount);
		pTruck->setStatringtime(simulationTime);
		check = true;
		//puts cargo into a priQ to get them sorted by distance
		for (int i = 0; i < movingCount; i++) {

			pCargo = waitingnormalCargos.deleteFront();
			pCargo->setWaitingTime(simulationTime - pCargo->getPreperationTime());
			pCargo->setTruckID(pTruck->getID());
			loadTime += pCargo->getLoadTime();
			cargoQ.enqueue(pCargo);//used to sort by distance
		}
		//starts enqueuing cargo into the truck and sets it's CDT
		for (int i = 0; i < movingCount; i++)
		{
			pCargo = cargoQ.dequeue();
			hours = simulationTime.getDay() * 24 + simulationTime.getHour() + ceil(float(pCargo->getDD()) / pTruck->getSpeed()) + pCargo->getLoadTime() + addedHours + loadTime;
			Time cdt(hours / 24, hours % 24);
			pCargo->setCDT(cdt);
			pTruck->addCargo(pCargo);
			addedHours += pCargo->getLoadTime();

		}
		distance = pCargo->getDD();
		hours = hours + ceil(float(distance) / pTruck->getSpeed());
		Time ending(hours / 24, hours % 24);
		pTruck->setEndingHours(ending);
		pTruck->incrementActiveTime(pCargo->getCDT() - pTruck->getstartingtime());
		pTruck->setEndingHours(ending);
		pTruck->setLoadTime(loadTime);
		//for day shift it dequeus the truck from empty after it has been filled
		if (simulationTime.getHour() < 23 && simulationTime.getHour() >= 5)
		switch (pTruck->getType())
		{
		case normalTruck:emptynormalTrucks.dequeue();
			break;
		case vipTruck:emptyvipTrucks.dequeue();
			break;

		default:
			break;
		}

	}
	else check = false;

	return check;
}


bool Company::LoadSpecialCargo(bool maxW = 0)
{
	//checks for loadiong rule
	if (!waitingspecialCargos.getCount() || loadingspecialTruck || emptyspecialTrucks.isEmpty())return false;
	bool check = false;
	Cargo* pCargo = nullptr;
	Truck* pTruck = nullptr;
	pQueue2 <Cargo*>cargoQ;
	int hours, addedHours = 0;
	int countCargo = waitingspecialCargos.getCount();
	if (simulationTime.getHour() < 23 && simulationTime.getHour() >= 5) {
		if (emptyspecialTrucks.front() && countCargo > 0 && (emptyspecialTrucks.front()->getCapacity() == countCargo || maxW))
			pTruck = emptyspecialTrucks.dequeue();
	}
	else
	{
		//gets a car that can work in night shift
		pTruck = getNightCar(emptyspecialTrucks, countCargo);

	}
	if (!pTruck)return 0;

	int countmoving = 0;
	if (maxW && waitingspecialCargos.getCount() < pTruck->getCapacity()) {
		countmoving = waitingspecialCargos.getCount();

	}
	else
		countmoving = pTruck->getCapacity();
	int loadTime = 0, distance = 0;
	if (waitingspecialCargos.getCount() >= pTruck->getCapacity() || maxW)
	{
		cargoQ.initialize(countmoving);
		loadingspecialTruck = pTruck;
		pTruck->incrementAllDeliveries(countmoving);
		pTruck->setStatringtime(simulationTime);
		//puts cargo into a priQ to get them sorted by distance
		for (int i = 0; i < countmoving; i++) {

			pCargo = waitingspecialCargos.dequeue();
			pCargo->setWaitingTime(simulationTime - pCargo->getPreperationTime());
			pCargo->setTruckID(pTruck->getID());
			loadTime += pCargo->getLoadTime();
			cargoQ.enqueue(pCargo);//used to sort by distance
		}
		//starts enqueuing cargo into the truck and sets it's CDT
		for (int i = 0; i < countmoving; i++)
		{
			pCargo = cargoQ.dequeue();
			hours = simulationTime.getDay() * 24 + simulationTime.getHour() + ceil(float(pCargo->getDD()) / pTruck->getSpeed()) + pCargo->getLoadTime() + addedHours + loadTime;
			Time cdt(hours / 24, hours % 24);
			pCargo->setCDT(cdt);
			pTruck->addCargo(pCargo);
			addedHours = +pCargo->getLoadTime();
		}
		distance = pCargo->getDD();
		hours = hours + ceil(float(distance) / pTruck->getSpeed());
		Time ending(hours / 24, hours % 24);
		pTruck->setEndingHours(ending);
		pTruck->incrementActiveTime(pCargo->getCDT() - pTruck->getstartingtime());
		pTruck->setEndingHours(ending);
		pTruck->setLoadTime(loadTime);
	}
	else
		check = false;

	return check;

}


bool Company::loadVipCargo(bool maxW = 0)
{
	//checks for loadiong rule
	if (!waitingvipCargos.getCount() || loadingvipTruck || (emptynormalTrucks.isEmpty() && emptyvipTrucks.isEmpty() && emptyspecialTrucks.isEmpty()))return false;
	bool check = false;
	Cargo* pCargo = nullptr;
	Truck* pTruck = nullptr;
	pQueue2<Cargo*>cargoQ;
	int hours, addedHours = 0;
	int count = waitingvipCargos.getCount();
	if (simulationTime.getHour() < 23 && simulationTime.getHour() >= 5) {
		if (!emptyvipTrucks.isEmpty()) {
			pTruck = emptyvipTrucks.front();
		}
		else if (!emptynormalTrucks.isEmpty()) {
			pTruck = emptynormalTrucks.front();
		}
		else if (!emptyspecialTrucks.isEmpty()) {
			pTruck = emptyspecialTrucks.front();
		}
	}
	else
	{
		//gets a car that can work in night shift
		pTruck = getNightCar(emptyvipTrucks, count);
		if (!pTruck)
			pTruck = getNightCar(emptynormalTrucks, count);
		if (!pTruck)
			pTruck = getNightCar(emptyspecialTrucks, count);
	}
	if (!pTruck)return 0;

	if (pTruck->getCapacity() <= waitingvipCargos.getCount() || maxW) {
		int movingCargo = 0;
		if (maxW) {
			movingCargo = waitingvipCargos.getCount();
		}
		else {
			movingCargo = pTruck->getCapacity();
		}
		int  loadTime = 0, distance = 0;
		cargoQ.initialize(movingCargo);
		pTruck->incrementAllDeliveries(movingCargo);
		pTruck->setStatringtime(simulationTime);
		loadingvipTruck = pTruck;
		check = true;
		//puts cargo into a priQ to get them sorted by distance
		for (int i = 0; i < movingCargo; i++) {

			pCargo = waitingvipCargos.dequeue();
			pCargo->setWaitingTime(simulationTime - pCargo->getPreperationTime());
			pCargo->setTruckID(pTruck->getID());
			loadTime += pCargo->getLoadTime();
			cargoQ.enqueue(pCargo);//used to sort by distance
		}
		//starts enqueuing cargo into the truck and sets it's CDT
		for (int i = 0; i < movingCargo; i++)
		{
			pCargo = cargoQ.dequeue();
			hours = simulationTime.getDay() * 24 + simulationTime.getHour() + ceil(float(pCargo->getDD()) / pTruck->getSpeed()) + pCargo->getLoadTime() + addedHours + loadTime;
			Time cdt(hours / 24, hours % 24);
			pCargo->setCDT(cdt);
			pTruck->addCargo(pCargo);
			addedHours = +pCargo->getLoadTime();
		}
		distance = pCargo->getDD();
		hours = hours + ceil(float(distance) / pTruck->getSpeed());
		Time ending(hours / 24, hours % 24);
		pTruck->setEndingHours(ending);
		pTruck->incrementActiveTime(pCargo->getCDT() - pTruck->getstartingtime());
		pTruck->setEndingHours(ending);
		pTruck->setLoadTime(loadTime);
		//for day shift it dequeus the truck from empty after it has been filled
		if (simulationTime.getHour() < 23 && simulationTime.getHour() >= 5)
			switch (pTruck->getType())
			{
			case vipTruck:emptyvipTrucks.dequeue();
				break;
			case normalTruck:emptynormalTrucks.dequeue();
				break;
			case specialTruck:emptyspecialTrucks.dequeue();
				break;
			default:
				break;
			}

	}
	else {
		check = false;
	}
	return check;

}

void Company::movingTruckstoDelivery(Queue<Truck*>& qTruck)
{
	int count = qTruck.getCount();
	Truck* pTruck = nullptr;
	Cargo* pCargo = nullptr;

	for (int i = 0; i < count; i++)
	{
		pTruck = qTruck.dequeue();
		while (pTruck->getFrontCargo() && simulationTime >= pTruck->getFrontCargo()->getCDT())
		{
			movingCargo--;
			pCargo = pTruck->removeCargo();
			deliveredCargo.enqueue(pCargo);
		}
		qTruck.enqueue(pTruck);
	}

}
bool Company::checkMaxW()
{
	Cargo* pCargo;
	pCargo = waitingnormalCargos.getFront();
	if (pCargo && simulationTime - pCargo->getPreperationTime() >= maxW) {

		LoadNormalCargo(1);

	}
	pCargo = waitingspecialCargos.getFront();
	if (pCargo && simulationTime - pCargo->getPreperationTime() >= maxW) {

		LoadSpecialCargo(1);

	}
	pCargo = waitingvipCargos.front();
	if (pCargo && simulationTime - pCargo->getPreperationTime() >= maxW) {

		loadVipCargo(1);

	}
	return 0;
}
bool Company::autopromotion()
{
	Cargo* pCargo;
	bool check = false;
	while (waitingnormalCargos.getCount() && simulationTime - waitingnormalCargos.getFront()->getPreperationTime() >= autoP * 24)
	{
		pCargo = waitingnormalCargos.deleteFront();
		waitingvipCargos.enqueue(pCargo);
		check = true;
		pCargo->incrementPromoted();
		pCargo->setType(vipCargo);
	}
	return check;
}

void Company::moveTruckToEmptyOrCheckup(Queue<Truck*>& qTruck)
{
	if (qTruck.getCount() == 0)return;

	int count = qTruck.getCount();
	Truck* pTruck = nullptr;
	for (int i = 0; i < count; i++)
	{
		pTruck = qTruck.dequeue();
		if ((pTruck->getEndingTime() - simulationTime) <= 0) {

			pTruck->incrementJ();
			if (pTruck->getJourney() % numJorneys == 0)
			{
				pTruck->setStartCheckUp(simulationTime);
				switch (pTruck->getType())
				{
				default:
				case vipTruck:
					checkupvipTrucks.enqueue(pTruck);
					break;
				case specialTruck:
					checkupspecialTrucks.enqueue(pTruck);
					break;
				case normalTruck:
					checkupnormalTrucks.enqueue(pTruck);
					break;
				}

			}
			else
			{
				switch (pTruck->getType())
				{
				default:
				case vipTruck:
					emptyvipTrucks.enqueue(pTruck);
					break;
				case specialTruck:
					emptyspecialTrucks.enqueue(pTruck);
					break;
				case normalTruck:
					emptynormalTrucks.enqueue(pTruck);
					break;
				}

			}

			continue;
		}

		qTruck.enqueue(pTruck);
	}



}



//2ady implementation
//it's left to add in the three while loop && with a condition
//the condition is:= movingnormalTrucks.getFront().timetruckfinishmoving() == simulationtime
/*
while (movingnormalTrucks.getFront() )
{
	if (movingnormalTrucks.getFront()->getJourney() == numJorneys)
	{
		movingnormalTrucks.getFront()->setJ(0);
		movingnormalTrucks.getFront()->setStartCheckUp(simulationTime);
		checkupnormalTrucks.enqueue(movingnormalTrucks.getFront());
	}
	else
	{
		emptynormalTrucks.enqueue(movingnormalTrucks.getFront());
	}
	movingnormalTrucks.dequeue();
}
while (movingspecialTrucks.getFront())
{
	if (movingspecialTrucks.getFront()->getJourney() == numJorneys)
	{
		movingspecialTrucks.getFront()->setJ(0);
		movingspecialTrucks.getFront()->setStartCheckUp(simulationTime);
		checkupspecialTrucks.enqueue(movingspecialTrucks.getFront());
	}
	else
	{
		emptyspecialTrucks.enqueue(movingspecialTrucks.getFront());
	}
	movingspecialTrucks.dequeue();
}
while (movingvipTrucks.getFront())
{
	if (movingvipTrucks.getFront()->getJourney() == numJorneys)
	{
		movingvipTrucks.getFront()->setJ(0);
		movingvipTrucks.getFront()->setStartCheckUp(simulationTime);
		checkupvipTrucks.enqueue(movingvipTrucks.getFront());
	}
	else
	{
		emptyvipTrucks.enqueue(movingvipTrucks.getFront());
	}
	movingvipTrucks.dequeue();
}
*/

void Company::checkupToEmpty()
{
	Truck* pTruck = nullptr;
	while (checkupnormalTrucks.getFront() && simulationTime.getDay() * 24 + simulationTime.getHour() >= checkupnormalTrucks.getFront()->getEndCheckupTime())
	{
		pTruck = checkupnormalTrucks.dequeue();
		switch (pTruck->getType())
		{
		case normalTruck:
			emptynormalTrucks.enqueue(pTruck);
			break;
		case vipTruck:
			emptyvipTrucks.enqueue(pTruck);
			break;
		default:
			break;
		}
	}
	while (checkupspecialTrucks.getFront() && simulationTime.getDay() * 24 + simulationTime.getHour() >= checkupspecialTrucks.getFront()->getEndCheckupTime())
	{
		emptyspecialTrucks.enqueue(checkupspecialTrucks.dequeue());

	}
	while (checkupvipTrucks.getFront() && simulationTime.getDay() * 24 + simulationTime.getHour() >= checkupvipTrucks.getFront()->getEndCheckupTime())
	{
		pTruck = checkupvipTrucks.dequeue();
		switch (pTruck->getType())
		{
		case vipTruck:
			emptyvipTrucks.enqueue(pTruck);
			break;
		case specialTruck:
			emptyspecialTrucks.enqueue(pTruck);
			break;
		case normalTruck:
			emptynormalTrucks.enqueue(pTruck);
			break;
		default:
			break;
		}
	}
}

bool Company::decrementLoadTime(Truck* pTruck)
{
	if (!pTruck)
		return 0;
	return pTruck->decrenmetntLoading();

}

Truck* Company::getNightCar(pQueue<Truck*>& qTruck, int count)//count is the count of cargo waiting
{
	int countTruck = qTruck.getCount();
	Truck* pTruck = nullptr;
	Truck* truckReturn = nullptr;
	Queue<Truck*>temp;
	bool found = 0;
	for (int i = 0; i < countTruck; i++)
	{
		temp.enqueue(qTruck.dequeue());

	}
	for (int i = 0; i < countTruck; i++)
	{
		pTruck = temp.dequeue();
		if (pTruck->getNightShift() && pTruck->getCapacity() <= count && !found) {
			truckReturn = pTruck;
			found = 1;
		}

		else
			qTruck.enqueue(pTruck);
	}
	return truckReturn;
}

void Company::checkFailure()
{
	Cargo* pCargo;
	Truck* pTruck;
	for (int i = 0; i < movingnormalTrucks.getCount(); i++)
	{
		pTruck = movingnormalTrucks.dequeue();
		pTruck->setTruckFailure();
		if (pTruck->getTruckFailure() == 34)
		{
			//moved to checkup
			//waiting cargos to be assigned later to trucks
			pTruck->setStartCheckUp(simulationTime);
			pTruck->incrementJ();
			checkupnormalTrucks.enqueue(pTruck);//has to switch on truck type (important)
			int count = pTruck->getCargoNum();
			movingCargo -= count;
			pTruck->incrementAllDeliveries(-count);
			for (int i = 0; i < count; i++)
			{
				pCargo = pTruck->removeCargo();
				waitingnormalCargos.insertFront(pCargo);
			}



		}
		else
		{
			movingnormalTrucks.enqueue(pTruck);
		}
	}

	for (int i = 0; i < movingspecialTrucks.getCount(); i++)
	{
		pTruck = movingspecialTrucks.dequeue();
		pTruck->setTruckFailure();
		if (pTruck->getTruckFailure() == 34)//has to change
		{
			//moved to checkup
			//waiting cargos to be assigned later to trucks
			pTruck->setStartCheckUp(simulationTime);
			pTruck->incrementJ();
			checkupspecialTrucks.enqueue(pTruck);
			int count = pTruck->getCargoNum();
			movingCargo -= count;
			pTruck->incrementAllDeliveries(-count);
			for (int i = 0; i < count; i++)
			{
				pCargo = pTruck->removeCargo();
				waitingspecialCargos.enqueue(pCargo);
			}
		}
		else
		{
			movingspecialTrucks.enqueue(pTruck);
		}
	}

	for (int i = 0; i < movingvipTrucks.getCount(); i++)
	{
		pTruck = movingvipTrucks.dequeue();
		pTruck->setTruckFailure();
		if (pTruck->getTruckFailure() == 34)
		{
			//moved to checkup
			//waiting cargos to be assigned later to trucks

			pTruck->setStartCheckUp(simulationTime);
			pTruck->incrementJ();
			checkupvipTrucks.enqueue(pTruck);
			int count = pTruck->getCargoNum();
			movingCargo -= count;
			pTruck->incrementAllDeliveries(-count);
			for (int i = 0; i < count; i++)
			{
				pCargo = pTruck->removeCargo();
				waitingvipCargos.enqueue(pCargo);
			}
		}
		else
		{
			movingvipTrucks.enqueue(pTruck);
		}
	}
}

void Company::loadingToMoving(Truck*& pTruck)
{
	if (!pTruck)return;
	movingCargo += pTruck->getCargoNum();
	pTruck->setMovingTime(simulationTime);
	//pTruck->setCDTcargo();
	switch (pTruck->getFrontCargo()->getType())
	{
	case vipTruck:
		movingvipTrucks.enqueue(pTruck);
		break;
	case specialTruck:
		movingspecialTrucks.enqueue(pTruck);
		break;
	case normalTruck:
		movingnormalTrucks.enqueue(pTruck);
	default:
		break;
	}

	//set waiting time for all cargos
	//need to set cdt for cargo here :(
	pTruck = nullptr;

}
/*
	if (simulationTime.getDay() * 24 + simulationTime.getHour() == pTruck->getEndCheckupTime())
	{
		if (pTruck->getType() == vipTruck)
		{
			emptyvipTrucks.enqueue(pTruck);
		}
		else if (pTruck->getType() == specialTruck)
		{
			emptyspecialTrucks.enqueue(pTruck);
		}
		else if (pTruck->getType() == normalTruck)
		{
			emptynormalTrucks.enqueue(pTruck);
		}
	}
	*/

bool Company::promote(int key, int extraCost)
{
	Cargo* ptr = NULL;
	if (waitingnormalCargos.Delete(key, ptr)) {
		ptr->incrementCargo(extraCost);
		ptr->setType(vipCargo);
		waitingvipCargos.enqueue(ptr); //Go to vip list
		return true;
	}
	else return false;



}

bool Company::cancel(int key)
{
	Cargo* ptr = NULL;
	return (waitingnormalCargos.Delete(key, ptr));
	//now ptr has the canceled cargo data
	//Maybe want to save it in PHASE 2
}
void Company::Simulation()
{
	simulationTime.incrementTime(); //increments time with needed validations
	if (simulationTime.getDay() == 1 && simulationTime.getHour() == 23) {
		Cargo* ptr = NULL; //for debug purposes only
	}
	if (decrementLoadTime(loadingvipTruck)) {
		loadingToMoving(loadingvipTruck);
	}
	if (decrementLoadTime(loadingspecialTruck)) {
		loadingToMoving(loadingspecialTruck);
	}
	if (decrementLoadTime(loadingnormalTruck)) {
		loadingToMoving(loadingnormalTruck);
	}
	checkupToEmpty();
	movingTruckstoDelivery(movingvipTrucks);
	movingTruckstoDelivery(movingspecialTrucks);
	movingTruckstoDelivery(movingnormalTrucks);
	moveTruckToEmptyOrCheckup(movingvipTrucks);
	moveTruckToEmptyOrCheckup(movingspecialTrucks);
	moveTruckToEmptyOrCheckup(movingnormalTrucks);
	checkFailure();
	activeHours++; //increments step that should be checked each five steps (moiving->delivered)


	while (eventList.getFront() && simulationTime.getDay() >= eventList.getFront()->getTime().getDay() && simulationTime.getHour() >= eventList.getFront()->getTime().getHour())
	{
		executeEvent();
	}

	assignCargo();
	autopromotion();
	checkMaxW();





	if (modeType == interactiveMode) //this if condition is the main difference between Interactive and Step-By-Step
	{
		pUI->interactiveCondition();
	}
	if (modeType != silentMode)
	{
		pUI->printCurrentTime(simulationTime);          //prints current time
	///////////////////////////////////////////////////////////////////////////////////////////////////OUTPUT
	//waiting output
		pUI->printWaitingCargos(getWaitingCount());
		pUI->printNormalWaitingCargos(waitingnormalCargos);
		pUI->printSpecialWaitingCargos(waitingspecialCargos);
		pUI->printVipWaitingCargos(waitingvipCargos);
		pUI->printEndl();
		//seperate with hyphens
		pUI->printHyphens();
		//loading output
		pUI->printLoadingTrucks(getLoadingCount());
		pUI->printNormalLoadingTrucks(loadingnormalTruck);  //problem (in logic)
		pUI->printSpecialLoadingTrucks(loadingspecialTruck); //problem
		pUI->printVipLoadingTrucks(loadingvipTruck);     //problem
		pUI->printEndl();
		//seperate with hyphens
		pUI->printHyphens();
		//empty output
		pUI->printEmptyTrucksCount(getEmptyCount());
		pUI->printEmptyTrucks(emptyvipTrucks, emptyspecialTrucks, emptynormalTrucks);
		pUI->printEndl();
		//seperate with hyphens
		pUI->printHyphens();
		//moving output
		pUI->printMovingCargos(getMovingCount());
		pUI->printNormalMovingCargos(movingnormalTrucks);  //problem (in logic)
		pUI->printSpecialMovingCargos(movingspecialTrucks); //problem
		pUI->printVipMovingCargos(movingvipTrucks);	 //problem
		pUI->printEndl();
		//seperate with hyphens
		pUI->printHyphens();
		//checkup output
		pUI->printIncheckupTrucks(getCheckupCount());
		pUI->printNormalIncheckupTrucks(checkupnormalTrucks);	//problem (in truck print overloading)
		pUI->printSpecialIncheckupTrucks(checkupspecialTrucks); //problem
		pUI->printVipIncheckupTrucks(checkupvipTrucks);         //problem
		pUI->printEndl();
		//seperate with hyphens
		pUI->printHyphens();
		//delivered output
		pUI->printDeliveredCargos(getDeliveredCount());
		pUI->printDeliveredCargoQueues(deliveredCargo);
		pUI->printEndl();
		///////////////////////////////////////////////////////////////////////////////////////////////////OUTPUT
		if (modeType == stepbystepMode)
		{
			pUI->Sleep();
		}
	}

}

void Company::setMode(int mode)
{
	switch (mode)
	{
	case 0: modeType = interactiveMode; break;
	case 1: modeType = stepbystepMode; break;
	default: modeType = silentMode; break; //Assuming that the defalut mode is silent mode because
	}                                      //it does not print any simulation steps on the console
										  //it produces only an output file which is common in all modes
}

void Company::eventToCargo()
{
	Event* ptr;
	Cargo* pCargo = NULL;
	for (int i = 0; i < numEvents; i++)
	{
		ptr = eventList.getFront();
		ptr->execute(pCargo, this);
		if (pCargo) {
			cargos type = pCargo->getType();
			switch (type)
			{
				//case vipCargo:break;
			case specialCargos: waitingspecialCargos.enqueue(pCargo); break;
			case normalCargos: waitingnormalCargos.insertEnd(pCargo); break;
			case vipCargo: waitingvipCargos.enqueue(pCargo); break;

			}
		}
		eventList.dequeue();
		//Cargo(Time prep, int id, int load, char type, int dist, int price);
	}
}

bool Company::checkSimulationworking()
{
	if (getWaitingCount() == 0 && eventList.getCount() == 0 && getMovingCount() == 0 && getLoadingCount() == 0 && getCheckUpCount() == 0 && getEmptyCount() == getNumberTrucks())
	{
		if (modeType == silentMode) //condition on number of waiting == 0 and number of events == 0
		{
			pUI->SilentModeOutput();
		}
		calcCargoAverageWaitingTime();
		calcAutopromotedPercentage();
		calcAverageTrucksActiveTime();
		calcAverageTrucksUtilizationTime();
		outputFile();
		desructCompany();
		return false;
	}
	return true;
}

void Company::printData() const
{
	cout << endl << numNormal << endl << numSpecial << endl << numVip << endl << speedNormal << endl << speedSpecial << endl
		<< speedVip << endl
		<< capNormal << endl << capSpecial << endl << capVIp
		<< endl << numJorneys << endl << checkNormal << endl << checkSpecial << endl << checkVip << endl << autoP << endl << maxW
		<< endl << numEvents;
}

void Company::printEvent()
{
	while (!eventList.isEmpty())
	{
		cout << eventList.getFront()->getTime().getDay() << endl;
		eventList.dequeue();
	}
}

int Company::getWaitingCount()
{
	return (waitingnormalCargos.getCount() + waitingspecialCargos.getCount() + waitingvipCargos.getCount());
}

int Company::getLoadingCount()
{
	int count = 0;
	if (loadingnormalTruck)
		count++;
	if (loadingspecialTruck)
		count++;
	if (loadingvipTruck)
		count++;

	return count;
}

int Company::getNormalEmptyCount()
{
	return emptynormalTrucks.getCount();
}

int Company::getSpecialEmptyCount()
{
	return emptyspecialTrucks.getCount();
}

int Company::getVipEmptyCount()
{
	return emptyvipTrucks.getCount();
}

int Company::getEmptyCount()
{
	return (emptynormalTrucks.getCount() + emptyspecialTrucks.getCount() + emptyvipTrucks.getCount());
}

int Company::getMovingCount()
{
	return movingCargo;
}

int Company::getCheckupCount()
{
	return (checkupnormalTrucks.getCount() + checkupspecialTrucks.getCount() + checkupvipTrucks.getCount());
}

int Company::getNormalDeliveredCount()
{
	int normalDeliveredCount = 0;
	Cargo* pCargo;
	for (int i = 0; i < getDeliveredCount(); i++)
	{
		pCargo = deliveredCargo.dequeue();
		if (pCargo->getType() == normalCargos)
		{
			normalDeliveredCount++;
		}
		deliveredCargo.enqueue(pCargo);
	}
	return normalDeliveredCount;
}


int Company::getSpecialDeliveredCount()
{
	int specialDeliveredCount = 0;
	Cargo* pCargo;
	for (int i = 0; i < getDeliveredCount(); i++)
	{
		pCargo = deliveredCargo.dequeue();
		if (pCargo->getType() == specialCargos)
		{
			specialDeliveredCount++;
		}
		deliveredCargo.enqueue(pCargo);
	}
	return specialDeliveredCount;
}

int Company::getVipDeliveredCount()
{
	int vipDeliveredCount = 0;
	Cargo* pCargo;
	for (int i = 0; i < getDeliveredCount(); i++)
	{
		pCargo = deliveredCargo.dequeue();
		if (pCargo->getType() == vipCargo)
		{
			vipDeliveredCount++;
		}
		deliveredCargo.enqueue(pCargo);
	}
	return vipDeliveredCount;
}

int Company::getCheckUpCount()
{
	return checkupnormalTrucks.getCount() + checkupspecialTrucks.getCount() + checkupvipTrucks.getCount();
}

int Company::getNumberTrucks() const
{
	return numNormal + numSpecial + numVip;
}

int Company::getDeliveredCount()
{
	return deliveredCargo.getCount();

}


void Company::executeEvent()
{
	Event* ptr;
	Cargo* pCargo = NULL;
	ptr = eventList.getFront();
	if (ptr) {
		ptr->execute(pCargo, this);
		eventList.dequeue();
		if (pCargo) {
			cargos type = pCargo->getType();
			switch (type)
			{
				//case vipCargo:break;
			case specialCargos: waitingspecialCargos.enqueue(pCargo); break;
			case normalCargos: waitingnormalCargos.insertEnd(pCargo); break;
			case vipCargo: waitingvipCargos.enqueue(pCargo); break;

			}
		}

	}
}

void Company::printEvents()
{
	Cargo* ptr = NULL;
	while (waitingnormalCargos.getCount()) {
		ptr = waitingnormalCargos.deleteFront();
		cout << ptr->getID() << endl;

	}
}

void Company::outputFile()
{
	//ios::out (write)
	//ios::app (app)
	//ios::in (read)
	string filename = pUI->chooseFilename();
	fstream myFile;
	myFile.open(filename, ios::out);//write
	if (myFile.is_open())
	{
		Cargo* pCargo;
		myFile << "CDT\tCID\tPT\tWT\tTID " << endl;
		int count = getDeliveredCount();
		for (int i = 0; i < count; i++)
		{
			pCargo = deliveredCargo.dequeue();
			myFile << pCargo->getCDT().getDay() << ":" << pCargo->getCDT().getHour() << "\t" << pCargo->getID() << "\t" << pCargo->getPreperationTime().getDay() << ":" << pCargo->getPreperationTime().getHour() << "\t" << pCargo->getWaitingTime() << "\t" << pCargo->getTruckID() << endl;
			deliveredCargo.enqueue(pCargo);
		}
		myFile << "............................" << endl;
		myFile << "............................" << endl;
		myFile << "Cargos: " << getDeliveredCount() << " [N: " << getNormalDeliveredCount() << ", S: " << getSpecialDeliveredCount() << ", V : " << getVipDeliveredCount() << "]" << endl;
		myFile << "Cargo Avg Wait = " << averageWaitingTime.getDay() << ":" << averageWaitingTime.getHour() << endl; //call avg wait function
		myFile << "Auto-promoted Cargos: " << autopromotedPercentage << "%" << endl; //call auto-promoted cargos
		myFile << "Trucks: " << numNormal + numSpecial + numVip << " [N:" << numNormal << ", S : " << numSpecial << ", V : " << numVip << "]" << endl; //call truck function count and N,S,V count
		myFile << "Avg Active time = " << averageTrucksActiveTime << "%" << endl; //call average active time function
		myFile << "Avg utilization =  " << averageTrucksUtilizationTime << "%" << endl; //call averag utilization
		myFile.close();
	}
}

void Company::calcCargoAverageWaitingTime()
{
	Cargo* pCargo;
	int totalWaitingTime = 0;
	for (int i = 0; i < getDeliveredCount(); i++)
	{
		pCargo = deliveredCargo.dequeue();
		totalWaitingTime += pCargo->getWaitingTime();
		deliveredCargo.enqueue(pCargo);
	}
	int averageWaitingtime = totalWaitingTime / getDeliveredCount();
	averageWaitingTime.Setday(averageWaitingtime / 24);
	averageWaitingTime.Sethour(averageWaitingtime - 24 * averageWaitingTime.getDay());
}

void Company::calcAutopromotedPercentage()
{
	Cargo* pCargo;
	int totalAutopromoted = 0;
	for (int i = 0; i < getDeliveredCount(); i++)
	{
		pCargo = deliveredCargo.dequeue();
		totalAutopromoted += pCargo->getPromoted();
		deliveredCargo.enqueue(pCargo);
	}
	if (getNormalDeliveredCount() != 0)
		autopromotedPercentage = (float(totalAutopromoted) / getNormalDeliveredCount()) * 100;
	else
		autopromotedPercentage = 0;
}

void Company::calcAverageTrucksActiveTime()
{

	Truck* pTruck;

	int totalActiveTime = 0;
	int totalTrucks = numNormal + numSpecial + numVip;
	Queue<Truck*>temp;
	int count = getNormalEmptyCount();
	for (int i = 0; i < count; i++)
	{
		temp.enqueue(emptynormalTrucks.dequeue());

	}
	for (int i = 0; i < count; i++)
	{
		pTruck = temp.dequeue();
		totalActiveTime += pTruck->getActiveTime();
		emptynormalTrucks.enqueue(pTruck);
	}
	
	 count = getSpecialEmptyCount();
	for (int i = 0; i < count; i++)
	{
		temp.enqueue(emptyspecialTrucks.dequeue());

	}

	for (int i = 0; i < count; i++)
	{
		pTruck = temp.dequeue();
		totalActiveTime += pTruck->getActiveTime();
		emptyspecialTrucks.enqueue(pTruck);
	}
	count = getVipEmptyCount();
	for (int i = 0; i < count; i++)
	{
		temp.enqueue(emptyvipTrucks.dequeue());

	}

	for (int i = 0; i < count; i++)
	{
		pTruck = temp.dequeue();
		totalActiveTime += pTruck->getActiveTime();
		emptyvipTrucks.enqueue(pTruck);
	}
	int simulationtime = simulationTime.getDay() * 24 + simulationTime.getHour();
	averageTrucksActiveTime = (totalActiveTime / (float(simulationtime) * totalTrucks)) * 100;
}

void Company::calcAverageTrucksUtilizationTime()
{
	Truck* pTruck;
	float totalUtilization = 0;
	float first, second;
	Queue<Truck*>temp;
	int count = getNormalEmptyCount();
	for (int i = 0; i < count; i++)
	{
		temp.enqueue(emptynormalTrucks.dequeue());

	}
	for (int i = 0; i < count; i++)
	{
		pTruck = temp.dequeue();
		if (pTruck->getJourney() > 0)
		{
			first = float(pTruck->getAlldeliveries()) / (pTruck->getCapacity() * pTruck->getJourney());
			second = float(pTruck->getActiveTime()) / (simulationTime.getDay() * 24 + simulationTime.getHour());
			totalUtilization += first * second;
		}
		emptynormalTrucks.enqueue(pTruck);
	}

	count = getSpecialEmptyCount();
	for (int i = 0; i < count; i++)
	{
		temp.enqueue(emptyspecialTrucks.dequeue());

	}

	for (int i = 0; i < count; i++)
	{
		pTruck = temp.dequeue();
		if (pTruck->getJourney() > 0)
		{
			first = float(pTruck->getAlldeliveries()) / (pTruck->getCapacity() * pTruck->getJourney());
			second = float(pTruck->getActiveTime()) / (simulationTime.getDay() * 24 + simulationTime.getHour());
			totalUtilization += first * second;
		}
		emptyspecialTrucks.enqueue(pTruck);
	}

	count = getVipEmptyCount();
	for (int i = 0; i < count; i++)
	{
		temp.enqueue(emptyvipTrucks.dequeue());

	}
	for (int i = 0; i < count; i++)
	{
		pTruck = temp.dequeue();
		if (pTruck->getJourney() > 0)
		{
			first = float(pTruck->getAlldeliveries()) / (pTruck->getCapacity() * pTruck->getJourney());
			second = float(pTruck->getActiveTime()) / (simulationTime.getDay() * 24 + simulationTime.getHour());
			totalUtilization += first * second;
		}
		emptyvipTrucks.enqueue(pTruck);
	}

	averageTrucksUtilizationTime = (float(totalUtilization) / (numNormal + numSpecial + numVip)) * 100;

}



void Company::desructCompany()
{
	deleteQueue(eventList);
	deleteQueue(waitingspecialCargos);
	deleteQueue(deliveredCargo);
	deleteQueue(checkupvipTrucks);
	deleteQueue(checkupspecialTrucks);
	deleteQueue(checkupnormalTrucks);


	deletePQueue(emptyvipTrucks);
	deletePQueue(emptyspecialTrucks);
	deletePQueue(emptynormalTrucks);


	deleteLinkedList(waitingnormalCargos);

}
