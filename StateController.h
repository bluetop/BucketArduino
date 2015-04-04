#pragma once
#include "TimeTracker.h"

class StateController
{
private:
	//Time limits in seconds
	int static const MAX_COOLING_TIME = 60;
	int static const MIN_OFF_TIME = 60;

	int currentState;
	int secondsPassed;
	float temperature;
	float targetTemperature;

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
	void setTemperature(int temperature);
	void setSecoundsPassed(int seconds);
	void setTargetTemperature(float maxTemp);
};

