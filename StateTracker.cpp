#include "StateTracker.h"
#include "Arduino.h"

//Statuses
//The bucket is currently cooling
#define STATE_COOLING = 10 
//The bucket is currently heating
#define STATE_HEATING = 12 
//The bucket has reached the max cooling time
//#define STATE_JUST_HEATED = 14 //The bucket has reached the max heating time

//The bucket is neither heating or cooling
#define STATE_OFF 15 

//Time limits in seconds
#define MAX_COOLING_TIME = 60
#define MAX_HEATING_TIME = 900


StateTracker::StateTracker()
{
	this->temperature = -1;
	this->secondsPassed = -1;
	this->currentState = STATE_OFF;
	this->targetTemperature = -1;
}


StateTracker::~StateTracker()
{


}

void StateTracker::setTemperature(int temperature) {
	this->setTemperature = temperature;
}

void StateTracker::setSecoundsPassed(int seconds) {
	this->secondsPassed = seconds;
}

void StateTracker::setTargetTemperature(float targetTemperature) {
	this->targetTemperature = targetTemperature;
}


int StateTracker::getState() {

	if (this->temperature == -1 || this->secondsPassed == -1 || this->targetTemperature == -1) {
		return STATE_OFF;  //not initialised yet
	}

	


}