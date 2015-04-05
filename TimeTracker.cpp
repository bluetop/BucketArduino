// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// author T Fawcet

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
