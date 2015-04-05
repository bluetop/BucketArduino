// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// author T Fawcet
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

