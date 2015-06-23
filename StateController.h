// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// author T Fawcet

#pragma once
#include "TimeTracker.h"
#include "Arduino.h"

class StateController
{ 
private:
	//Time limits in seconds
	int static const MAX_COOLING_TIME = 1800;
	int static const MIN_OFF_TIME = 30;

	int currentState;
	int secondsPassed;
	float beerTemperature;
	float targetTemperature;
	float ambientTemperature;

	TimeTracker* timeTracker;

	int changeState(int state);


public:
	StateController(TimeTracker* timeTracker);
	~StateController();

	//Statuses
	int static const STATE_COOLING = 10; //The bucket is currently cooling
	int static const STATE_HEATING = 12; //The bucket is currently heating
	int static const STATE_OFF = 15; //The bucket is neither heating or cooling
	

	int checkState();
	void setBeerTemperature(float temperature);
	void setSecoundsPassed(int seconds);
	void setTargetTemperature(float maxTemp);
	float getTargetTemperature();
	float getBeerTemperature();
	float getAmbientTemperature();
	void setAmbientTemperature(float temperature);
	String getDescription();
	String getState();
};

