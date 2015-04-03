
#include "Arduino.h"
#include "TimeTracker.h"


TimeTracker::TimeTracker()
{
	this->startTimeMillis = millis();
}


TimeTracker::~TimeTracker()
{
}

//Returns the number of seconds since TimeTracker was reset
int TimeTracker::getSeconds() {
	unsigned long now = millis();
	int secondsPassed = (now - startTimeMillis) / 1000;
	return secondsPassed;
}

//Resets the start time, so effectively resets the getSeconds() count to zero
void TimeTracker::reset() {
	this->startTimeMillis = millis();
}
