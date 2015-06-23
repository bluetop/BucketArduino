// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// author T Fawcet
#include "StateController.h"

StateController::StateController(TimeTracker* timeTracker)
{
	this->beerTemperature = -100;
	this->secondsPassed = -100; 
	this->currentState = STATE_OFF;
	this->targetTemperature = -100;
	this->timeTracker = timeTracker;
}


StateController::~StateController()
{


}

void StateController::setBeerTemperature(float temperature) {
	this->beerTemperature = temperature;
}


void StateController::setAmbientTemperature(float temperature) {
	this->ambientTemperature = temperature;
}

void StateController::setSecoundsPassed(int seconds) {
	this->secondsPassed = seconds;
}

void StateController::setTargetTemperature(float targetTemperature) {
	this->targetTemperature = targetTemperature;
}

float StateController::getTargetTemperature() {
	return this->targetTemperature;
}

float StateController::getBeerTemperature() {
	return this->beerTemperature;
}

float StateController::getAmbientTemperature() {
	return this->ambientTemperature;
}

//Updates the current status, and returns the passed status.
int StateController::changeState(int state) {
	if (state != this->currentState) {
		this->currentState = state;
		this->timeTracker->reset(); //Reseting time tracker as we have changed state
	}
	return state;
}

//returns a string description of the current state
String StateController::getState() {
	if (this->currentState == this->STATE_COOLING) {
		return "COOLING";
	}
	if (this->currentState == this->STATE_HEATING) {
		return "HEATING";
	}
	if (this->currentState == this->STATE_OFF) {
		return "OFF";
	}
	return "UNKNOWN";
}

int StateController::checkState() {
	this->secondsPassed = this->timeTracker->getSeconds();

	if (this->beerTemperature <= -1 || this->secondsPassed <= -1 || this->targetTemperature <= -1) {
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
		if (this->beerTemperature >(targetTemperature + 1)) {
			String debug = "Starting cooling: ";
			debug += this->secondsPassed;
			Serial.println(debug);
			//temperature is too hot, need to cool
			return this->changeState(STATE_COOLING);
		}
		if (this->beerTemperature < (targetTemperature - 1)) {
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
		if (this->beerTemperature <= (targetTemperature)) {
			//temperature is cold enough, need to stop cooling
			String debug = "Target temperature reached******** (was cooling)";
			Serial.println(debug);
			return this->changeState(STATE_OFF);
		}
	}

	if (this->currentState == STATE_HEATING) {
		if (this->beerTemperature >= (targetTemperature)) {
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
	result += ",";
	result += "  BeerTemp: ";
	result += beerTemperature;
	result += ",";
	result += "  AmbientTemp: ";
	result += ambientTemperature;
	result += ",";
	result += "  Seconds: ";
	result += secondsPassed;
	//result += "\n";
	result += "}";
	return result;
}
