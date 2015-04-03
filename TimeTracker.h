#pragma once

//Tracks the number of seconds between state changes
class TimeTracker
{
private: 
	unsigned long startTimeMillis;

public:
	TimeTracker();
	~TimeTracker();
	
	int getSeconds();

	void reset();
};

