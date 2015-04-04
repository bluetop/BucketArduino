
#include "StateController.h"
#include "Arduino.h"

StateController::StateController(TimeTracker* timeTracker)
{
	this->temperature = -1;
	this->secondsPassed = -1;
	this->currentState = STATE_OFF;
	this->targetTemperature = -1;
	this->timeTracker = timeTracker;
}


StateController::~StateController()
{


}

void StateController::setTemperature(int temperature) {
	this->temperature = temperature;
}

void StateController::setSecoundsPassed(int seconds) {
	this->secondsPassed = seconds;
}

void StateController::setTargetTemperature(float targetTemperature) {
	this->targetTemperature = targetTemperature;
}

//Updates the current status, and returns the passed status.
int StateController::changeState(int state) {
	if (state != this->currentState) {
		this->currentState = state;
		this->timeTracker->reset(); //Reseting time tracker as we have changed state
	}
	return state;
}


int StateController::checkState() {

	if (this->temperature == -1 || this->secondsPassed == -1 || this->targetTemperature == -1) {
		return this->changeState(STATE_OFF);  //not initialised yet
	}

	if (this->currentState == STATE_OFF) {
		if (this->secondsPassed < MIN_OFF_TIME) {
			//only just switched off, staying switched off until MIN_OFF_TIME passes 
			return this->changeState(STATE_OFF);
		}
		if (this->temperature > (targetTemperature + 1)) {
			//temperature is too hot, need to cool
			return this->changeState(STATE_COOLING);
		}
		if (this->temperature < (targetTemperature - 1)) {
			//temperature is too cold, need to heat
			return this->changeState(STATE_HEATING);
		}
	}

	if (this->currentState == STATE_COOLING && this->secondsPassed > MAX_COOLING_TIME) {
		return this->changeState(STATE_OFF);
	}

	if (this->currentState == STATE_COOLING || this->currentState == STATE_HEATING) {
		if (this->temperature >= (targetTemperature)) {
			//temperature is too hot, need to stop heating
			return this->changeState(STATE_OFF);
		}
		if (this->temperature <= (targetTemperature)) {
			//temperature is cold enough, need to stop cooling
			return this->changeState(STATE_OFF);
		}
	}

	
	return this->currentState;
}