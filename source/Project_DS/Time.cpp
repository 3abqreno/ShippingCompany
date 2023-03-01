#include "Time.h"

Time::Time(int d, int h)
{
	Setday(d);
	Sethour(h);
}

void Time::Setday(int d)
{
	day = d;
}

void Time::Sethour(int h)
{
	hour = h;
}

int Time::getDay() const
{
	return day;
}

int Time::getHour() const
{
	return hour;
}

void Time::incrementTime()
{
	hour++;
	if (hour == 24)
	{
		hour = 0;
		day++;
	}
}
int Time::operator-(Time other)
{
	return (this->day * 24 + this->hour) - (other.getDay() * 24 + other.getHour());
}

bool Time::operator>=(Time other)
{
	if ((this->getDay() >= other.getDay()) && this->getHour()>=other.getHour())
		return 1;
	else
		return 0;

}

bool Time::operator>(Time other)
{
	if ((this->day * 24 + this->hour) > (other.day * 24 + other.hour))
		return 1;
	else
		return 0;
}

bool Time::operator<(Time other)
{
	if ((this->day * 24 + this->hour) < (other.day * 24 + other.hour))
		return 1;
	else
		return 0;
}


