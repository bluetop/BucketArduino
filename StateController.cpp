// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// author T Fawcet
#include "StateController.h"

StateController::StateController(TimeTracker* timeTracker)
{
	this->bucketTemperature = -100;
	this->secondsPassed = -100;
	this->currentState = STATE_OFF;
	this->targetTemperature = -100;
	this->timeTracker = timeTracker;
}


StateController::~StateController()
{


}

void StateController::setBucketTemperature(float temperature) {
	this->bucketTemperature = temperature;
}


void StateController::setChillerBoxTemperature(float temperature) {
	this->chillerBoxTemperature = temperature;
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
	this->secondsPassed = this->timeTracker->getSeconds();

	if (this->bucketTemperature <= -1 || this->secondsPassed <= -1 || this->targetTemperature <= -1) {
		return this->changeState(STATE_OFF);  //not initialised yet
	}

	if (this->currentState == STATE_OFF) {
		if (this->secondsPassed < MIN_OFF_TIME) {
			String debug = "Min off time not reached reached: ";
			debug += MIN_OFF_TIME;
			debug += ", ";
			debug += this->secondsPassed;
			Serial.println(debug);
			//only just switched off, staying switched off until MIN_OFF_TIME passes 
			return this->changeState(STATE_OFF);
		}
		if (this->bucketTemperature >(targetTemperature + 1)) {
			String debug = "Starting cooling: ";
			debug += this->secondsPassed;
			Serial.println(debug);
			//temperature is too hot, need to cool
			return this->changeState(STATE_COOLING);
		}
		if (this->bucketTemperature < (targetTemperature - 1)) {
			String debug = "Starting heating: ";
			debug += this->secondsPassed;
			Serial.println(debug);
			//temperature is too cold, need to heat
			return this->changeState(STATE_HEATING);
		}
	}

	if (this->currentState == STATE_COOLING) { 
		if (this->secondsPassed > MAX_COOLING_TIME) {
			//as we are pumping cold water, don't want to dump temperature down too fast
			String debug = "Max cooling time reached: ";
			debug += MAX_COOLING_TIME;
			debug += ", ";
			debug += secondsPassed;
			Serial.println(debug);
			return this->changeState(STATE_OFF);
		}
		if (this->bucketTemperature <= (targetTemperature)) {
			//temperature is cold enough, need to stop cooling
			String debug = "Target temperature reached******** (was cooling)";
			Serial.println(debug);
			return this->changeState(STATE_OFF);
		}
	}

	if (this->currentState == STATE_HEATING) {
		if (this->bucketTemperature >= (targetTemperature)) {
			//temperature is cold enough, need to stop cooling
			String debug = "Target temperature reached******** (was heating)";
			Serial.println(debug);
			return this->changeState(STATE_OFF);
		}
	}
	
	return this->currentState;
}

String StateController::getDescription() {

	String result = "{\n  State: ";
	if (currentState == STATE_OFF) {
		result += "OFF,\n";
	}
	if (currentState == STATE_COOLING) {
		result += "COOLING,\n";
	}
	if (currentState == STATE_HEATING) {
		result += "HEATING,\n";
	}
	result += "  TargetTemp: ";
	result += targetTemperature;
//	result += ",\n";
	result += "  BucketTemp: ";
	result += bucketTemperature;
//	result += ",\n";
	result += "  ChillerBoxTemp: ";
	result += chillerBoxTemperature;
//	result += ",\n";
	result += "  Seconds: ";
	result += secondsPassed;
//	result += "\n";
	result += "}";
	return result;
}
