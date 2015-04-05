// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// author T Fawcet

#include <OneWire.h>
#include <DallasTemperature.h>
#include "TimeTracker.h"
#include "StateController.h"

#define ONE_WIRE_BUS 11
#define HEATING_PIN 8
#define COOLING_PIN 7

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

//tracks time between state changes
TimeTracker tracker;
StateController statusControl(&tracker);
int currentState;

void setup(void)
{
	// start serial port
	Serial.begin(9600);
	Serial.println("BrewBucket starting...");

	// Start up the library
	sensors.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement
	statusControl.setTargetTemperature(30.0);
	currentState = -1;
	
	pinMode(HEATING_PIN, OUTPUT);
	pinMode(COOLING_PIN, OUTPUT);
}

void loop(void)
{
	//update temperatures on the controller
	sensors.requestTemperatures(); // command to get temperatures
	float currentTemperature = sensors.getTempCByIndex(0);
	float chillerBoxTemperature = sensors.getTempCByIndex(1);
	statusControl.setBucketTemperature(currentTemperature);
	statusControl.setChillerBoxTemperature(chillerBoxTemperature);

	//check for a state change, and update the PINs accordingly
	int state = statusControl.checkState();
	if (state != currentState) {
		currentState = state;
		if (currentState == statusControl.STATE_COOLING) {
			digitalWrite(COOLING_PIN, HIGH);
			digitalWrite(HEATING_PIN, LOW);
		}
		if (currentState == statusControl.STATE_HEATING) {
			digitalWrite(COOLING_PIN, LOW);
			digitalWrite(HEATING_PIN, HIGH);
		}
		if (currentState == statusControl.STATE_OFF) {
			digitalWrite(COOLING_PIN, LOW);
			digitalWrite(HEATING_PIN, LOW);
		}
	}

	delay(1000);
	Serial.println(statusControl.getDescription());
}