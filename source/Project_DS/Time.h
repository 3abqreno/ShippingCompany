#pragma once
class Time
{
	int day, hour;
public:
	Time(int d = 0 ,int h = 0 ); //makes needed initializations
	void Setday(int d);			//sets day
	void Sethour(int h);		//sets hour
	int getDay()const;		    //returns day
	int getHour()const;			//returns hour
	void incrementTime();		//increments time for simulation with corner case
	int operator-(Time other);
	bool operator >=(Time other);
	bool operator >(Time other);
	bool operator <(Time other);
	
};

