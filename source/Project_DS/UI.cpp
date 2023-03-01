#include "UI.h"
#include<fstream>
#include <chrono>
#include <thread>
#include <iostream>
using namespace std;

ostream& operator << (ostream& out, const Cargo*& C)
{
	out << C->getID();
	return out;
}

UI::UI()
{

}



int UI::ChooseMode()
{
	cout << "This program can run in one of three modes:-" << endl << "Interactive, Step-By-Step, or Silent Mode" << endl;
	cout << "Enter a number to choose a mode from the following:" << endl;
	cout << "1- Interactive Mode" << endl;
	cout << "2- Step-By-Step Mode" << endl;
	cout << "3- Silent Mode" << endl;
	int mode; cin >> mode;
	return (--mode);
	system("cls");
}

void UI::printCurrentTime(Time simulationTime)
{
	cout << "Current Time (Day:Hour):" << simulationTime.getDay() << " : " << simulationTime.getHour() << endl;
}

void UI::printHyphens()
{
	cout << "-------------------------------------------------------" << endl;
}

void UI::Sleep()
{
	chrono::milliseconds timespan(1000); //waits in stepbystepMode by 1000 milliseconds which is one second
	this_thread::sleep_for(timespan);
	system("cls");
}

void UI::SilentModeOutput()
{
	cout << "Silent Mode" << endl;
	cout << "Simulation Starts..." << endl;
	cout << "Simulation ends, Output file created" << endl;
}

void UI::interactiveCondition()
{
	cin.get();							  //pauses for an input from the user (“Enter” key for example)
										  //to display the output of the next time.
}

void UI::printEndl()
{
	cout << endl;
}

void UI::printWaitingCargos(int w)
{
	cout << w << " Waiting Cargos: ";
}

void UI::printVipWaitingCargos(pQueue <Cargo*>& waitingQueue)
{
	cout << "{";
	waitingQueue.print();
	cout << "} ";
}

void UI::printSpecialWaitingCargos(Queue <Cargo*> waitingQueue)
{
	cout << "(";
	waitingQueue.PrintInfo();
	cout << ") ";
}

void UI::printNormalWaitingCargos(LinkedList <Cargo*> waitingQueue)
{
	cout << "[";
	waitingQueue.print();
	cout << "] ";
}

void UI::printLoadingTrucks(int l)
{
	cout << l << " Loading Trucks: ";
}

void UI::printVipLoadingTrucks(Truck* pTruck)
{
	if (!pTruck) {
		cout << "{} ";
		return;
	};
	//cout number here
	cout << pTruck->getID();
	cout << "{";
	pTruck->printCargo();
	cout << "} ";
}

void UI::printSpecialLoadingTrucks(Truck* pTruck)
{
	if (!pTruck) {
		cout << "() ";
		return;
	};
	//cout number here'
	cout << pTruck->getID();
	cout << "(";
	pTruck->printCargo();
	cout << ") ";
}

void UI::printNormalLoadingTrucks(Truck* pTruck)
{
	if (!pTruck) {
		cout << "[] ";
		return;
	};
	//cout number here
	cout << pTruck->getID();
	cout << "[";
	pTruck->printCargo();
	cout << "] ";
}

void UI::printEmptyTrucksCount(int e)
{
	cout << e << " Empty Trucks: ";
}

void UI::printEmptyTrucks(pQueue<Truck*>& vip, pQueue<Truck*>& special, pQueue<Truck*>& normal)
{
	Truck* emptyTruck;
	int countTruck = normal.getCount();
	Queue<Truck*>temp;
	for (int i = 0; i < countTruck; i++)
	{
		temp.enqueue(normal.dequeue());

	}
	for (int i = 0; i < countTruck; i++)
	{
		emptyTruck = temp.dequeue();
		cout << "[" << emptyTruck->getID() << "]";
		if (i < (countTruck - 1) || special.getCount()>0 || vip.getCount()>0)
			cout << ", ";
		normal.enqueue(emptyTruck);
	}

	
	countTruck = special.getCount();
	for (int i = 0; i < countTruck; i++)
	{
		temp.enqueue(special.dequeue());

	}
	for (int i = 0; i < countTruck; i++)
	{
		emptyTruck = temp.dequeue();
		cout << "(" << emptyTruck->getID() << ")";
		if (i < (countTruck - 1) || vip.getCount()>0)
			cout << ", ";
		special.enqueue(emptyTruck);
	}

	
	countTruck = vip.getCount();
	for (int i = 0; i < countTruck; i++)
	{
		temp.enqueue(vip.dequeue());

	}
	for (int i = 0; i < countTruck; i++)
	{
		emptyTruck = temp.dequeue();
		cout << "{" << emptyTruck->getID() << "}";
		if (i < (countTruck - 1))
			cout << ", ";
		vip.enqueue(emptyTruck);
	}
}

void UI::printVipEmptyTrucks(pQueue <Truck*>& emptyTrucks)
{
	Truck* emptyTruck;
	int countTruck = emptyTrucks.getCount();
	Queue<Truck*>temp;
	for (int i = 0; i < countTruck; i++)
	{
		temp.enqueue(emptyTrucks.dequeue());

	}
	for (int i = 0; i < countTruck; i++)
	{
		emptyTruck = temp.dequeue();
		cout << "{" << emptyTruck->getID()<<"}";
		if(i<(countTruck-1))
		cout<< ", ";
		emptyTrucks.enqueue(emptyTruck);
	}
}

void UI::printSpecialEmptyTrucks(pQueue <Truck*>& emptyTrucks)
{
	Truck* emptyTruck;
	int countTruck = emptyTrucks.getCount();
	Queue<Truck*>temp;
	for (int i = 0; i < countTruck; i++)
	{
		temp.enqueue(emptyTrucks.dequeue());

	}
	for (int i = 0; i < countTruck; i++)
	{
		emptyTruck = temp.dequeue();
		cout << "(" << emptyTruck->getID() << "), ";
		emptyTrucks.enqueue(emptyTruck);
	}
}

void UI::printNormalEmptyTrucks(pQueue <Truck*>& emptyTrucks)
{
	Truck* emptyTruck;
	int countTruck = emptyTrucks.getCount();
	Queue<Truck*>temp;
	for (int i = 0; i < countTruck; i++)
	{
		temp.enqueue(emptyTrucks.dequeue());

	}
	for (int i = 0; i < countTruck; i++)
	{
		emptyTruck = temp.dequeue();
		cout << "[" << emptyTruck->getID() << "], ";
		emptyTrucks.enqueue(emptyTruck);
	}
}

void UI::printMovingCargos(int m)
{
	cout << m << " Moving Cargos: ";
}

void UI::printVipMovingCargos(Queue <Truck*>& movingTrucks)
{
	int count = movingTrucks.getCount();
	Truck* pTruck = nullptr;
	for (int i = 0; i < count; i++)
	{
		pTruck = movingTrucks.dequeue();
		cout << pTruck->getID();
		cout << "{";
		pTruck->printCargo();
		cout << "} ";
		movingTrucks.enqueue(pTruck);
	}
}

void UI::printSpecialMovingCargos(Queue <Truck*>& movingTrucks)
{
	int count = movingTrucks.getCount();
	Truck* pTruck = nullptr;
	for (int i = 0; i < count; i++)
	{
		pTruck = movingTrucks.dequeue();
		cout << pTruck->getID();
		cout << "(";
		pTruck->printCargo();
		cout << ") ";
		movingTrucks.enqueue(pTruck);
	}
}

void UI::printNormalMovingCargos(Queue <Truck*>& movingTrucks)
{
	int count = movingTrucks.getCount();
	Truck* pTruck = nullptr;
	for (int i = 0; i < count; i++)
	{
		pTruck = movingTrucks.dequeue();
		cout << pTruck->getID();
		cout << "[";
		pTruck->printCargo();
		cout << "] ";
		movingTrucks.enqueue(pTruck);
	}

}

void UI::printIncheckupTrucks(int c)
{
	cout << c << " In-Checkup Trucks: ";
}

void UI::printVipIncheckupTrucks(Queue <Truck*>& checkupTrucks)
{
	Truck* pTruck = NULL;
	int count = checkupTrucks.getCount();
	cout << "{";
	for (int i = 0; i < count; i++)
	{
		pTruck = checkupTrucks.dequeue();
		cout << pTruck->getID();
		if(i<(count-1))
			cout<< ",";
		checkupTrucks.enqueue(pTruck);
	}
	cout << "} ";
}


void UI::printSpecialIncheckupTrucks(Queue <Truck*>& checkupTrucks)
{
	Truck* pTruck = NULL;
	int count = checkupTrucks.getCount();
	cout << "(";
	for (int i = 0; i < count; i++)
	{
		pTruck = checkupTrucks.dequeue();
		cout << pTruck->getID();
		if(i<(count-1))
		cout << ",";
		checkupTrucks.enqueue(pTruck);
	}
	cout << ") ";
}

void UI::printNormalIncheckupTrucks(Queue <Truck*>& checkupTrucks)
{
	Truck* pTruck = NULL;
	int count = checkupTrucks.getCount();
	cout << "[";
	for (int i = 0; i < count; i++)
	{
		pTruck = checkupTrucks.dequeue();
		cout << pTruck->getID();
		if(i<count-1)
		cout << ",";

		checkupTrucks.enqueue(pTruck);
	}
	cout << "] ";
}


void UI::printDeliveredCargos(int d)
{
	cout << d << " Delivered Cargos: ";
}

void UI::printDeliveredCargoQueues(Queue <Cargo*>& deliveredCargos)
{
	Cargo* deliveredCargo;
	for (int i = 0; i < deliveredCargos.getCount(); i++)
	{
		deliveredCargo = deliveredCargos.dequeue();
		if (deliveredCargo->getType() == normalCargos)
		{
			cout << "[" << deliveredCargo->getID() << "] ";
		}
		else if (deliveredCargo->getType() == specialCargos)
		{
			cout << "(" << deliveredCargo->getID() << ") ";
		}
		else if (deliveredCargo->getType() == vipCargo)
		{
			cout << "{" << deliveredCargo->getID() << "} ";
		}
		deliveredCargos.enqueue(deliveredCargo);
	}
}
string UI::chooseFilename()
{
	cout << endl;
	cout << "The company finished all its events" << endl;
	cout << "Please, Enter the output filename the data will be saved in: ";
	string filename;
	cin >> filename;
	filename = filename + ".txt";
	cout << "The data was saved to file " << filename << " successfully" << endl;
	return filename;
}
string UI::chooseInputFilename()
{
	cout << "Please, Enter the input filename the data will be taken from: ";
	string filename;
	cin >> filename;
	filename = filename + ".txt";
	cout << "The data was taken from file " << filename << " successfully" << endl;
	return filename;
}
//auxiliary needed at the beginning of phase 2
/*
void UI::printVipDeliveredCargos(Queue <Cargo*> deliveredCargos)
{
	cout << "{";
	deliveredCargos.PrintInfo();
	cout << "} ";
}

void UI::printSpecialDeliveredCargos(Queue <Cargo*> deliveredCargos)
{
	cout << "(";
	deliveredCargos.PrintInfo();
	cout << ") ";
}

void UI::printNormalDeliveredCargos(Queue <Cargo*> deliveredCargos)
{
	cout << "[";
	deliveredCargos.PrintInfo();
	cout << "] ";
}
*/