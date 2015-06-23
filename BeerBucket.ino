// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// author T Fawcet
//#include "Arduino.h"

#include <SPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_GFX.h>
//#include <Adafruit_PCD8544.h>
#include <Adafruit_ST7735.h> // Hardware-specific library
#include "TimeTracker.h"
#include "StateController.h"
#include "DisplayLCD.h"
#include "ESP8266.h"
#include "Wifi.h"
#include <SoftwareSerial.h>


#define  USER_SEL_VERSION         VERSION_22

#define ONE_WIRE_BUS A1  //4.6k pull up
#define HEATING_PIN A5
#define COOLING_PIN A4
#define BUTTON_HOTTER A0
#define BUTTON_COLDER A2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire* oneWire = new  OneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature* sensors = new  DallasTemperature(oneWire);

//tracks time between state changes
TimeTracker* tracker = new TimeTracker(0); 
StateController* statusControl = new StateController(tracker);
TimeTracker* temperatureTimer = new TimeTracker(10000);
TimeTracker* wifiTimer = new TimeTracker(60000);
TimeTracker* wifiResetTimer = new TimeTracker(900000);
TimeTracker* screenRefreshTimer = new TimeTracker(700);

boolean firstRun;
int currentState;

DisplayLCD* lcd;

//SoftwareSerial softSerial(WIFI_RX, WIFI_TX); /* RX:D3, TX:D2 */
//ESP8266 wifi(mySerial);
Wifi wifi;

int counterForWifi;
int counterForTemp;
int counterForScreen;
boolean coolButtonDown;
boolean hotButtonDown;
void setup(void)
{
	// start serial port 
	Serial.begin(9600);
	Serial.println("BrewBucket starting...");
	lcd = new DisplayLCD();

	// Start up the library
	Serial.println("sensors start");
	sensors->setResolution(10);
	sensors->begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement
	statusControl->setTargetTemperature(18.0);
	currentState = -1;

	pinMode(HEATING_PIN, OUTPUT);
	pinMode(COOLING_PIN, OUTPUT);
	pinMode(BUTTON_HOTTER, INPUT_PULLUP);
	pinMode(BUTTON_COLDER, INPUT_PULLUP);

	counterForWifi = 0;
	counterForTemp = 0;
	coolButtonDown = false;
	hotButtonDown = false;
	Serial.println("setup done");
	temperatureTimer->reset();
	firstRun = true;

	Serial.print("Wifi timeout: ");
	Serial.println(wifiTimer->getTimeout());
	Serial.print("Wifi reset timeout: ");
	Serial.println(wifiResetTimer->getTimeout());
}


void loop(void)
{
	//Serial.println("loop start");

	//update temperatures on the controller
	if (temperatureTimer->timeoutReached() || firstRun) {
		lcd->write("_SENSORS_", ST7735_WHITE);
		sensors->requestTemperatures(); // command to get temperatures
		float beerTemperature = sensors->getTempCByIndex(0);
		float ambientTemperature = sensors->getTempCByIndex(1);
		statusControl->setBeerTemperature(beerTemperature);
		statusControl->setAmbientTemperature(ambientTemperature);
		Serial.println(statusControl->getDescription());

		//check for a state change, and update the PINs accordingly
		int state = statusControl->checkState();
		if (state != currentState) {
			currentState = state;
			if (currentState == statusControl->STATE_COOLING) {
				digitalWrite(COOLING_PIN, HIGH);
				digitalWrite(HEATING_PIN, LOW);
			}
			if (currentState == statusControl->STATE_HEATING) {
				digitalWrite(HEATING_PIN, HIGH);
				digitalWrite(COOLING_PIN, LOW);
			}
			if (currentState == statusControl->STATE_OFF) {
				digitalWrite(COOLING_PIN, LOW);
				digitalWrite(HEATING_PIN, LOW);
			}
		}
		lcd->write("_SENSORS_", ST7735_BLACK);
	}

	//update the screen
	if (screenRefreshTimer->timeoutReached()) {
		lcd->writeState(statusControl->getState(), statusControl->getBeerTemperature(), statusControl->getTargetTemperature(), statusControl->getAmbientTemperature());
	}

	//sending state to server
	if (wifiTimer->timeoutReached()) {
		lcd->write("_WIFI-SENDING_", ST7735_WHITE);
		String result = wifi.sendData(statusControl->getState(), statusControl->getBeerTemperature(), statusControl->getTargetTemperature(), statusControl->getAmbientTemperature());
		Serial.println(result);
		lcd->write("_WIFI-SENDING_", ST7735_BLACK);
	}

	//reset wifi occationally to make sure it's working
	if (wifiResetTimer->timeoutReached()) {
		lcd->write("_WIFI-RESET_", ST7735_WHITE);
		wifi.reset();
		lcd->write("_WIFI-RESET_", ST7735_BLACK);
	}
	
	//read the buttons
	if (digitalRead(BUTTON_COLDER) == LOW && !coolButtonDown) {
		delay(50);
		if (digitalRead(BUTTON_COLDER) == LOW) {
			delay(50);
			if (digitalRead(BUTTON_COLDER) == LOW) {
				float targetTemp = statusControl->getTargetTemperature();
				targetTemp = targetTemp - 0.5;
				statusControl->setTargetTemperature(targetTemp);
				coolButtonDown = true;
			}
		}
	}
	else {
		coolButtonDown = false;
	}


	if (digitalRead(BUTTON_HOTTER) == LOW && !hotButtonDown) {
		delay(50);
		if (digitalRead(BUTTON_HOTTER) == LOW) {
			delay(50);
			if (digitalRead(BUTTON_HOTTER) == LOW) {
				float targetTemp = statusControl->getTargetTemperature();
				targetTemp = targetTemp + 0.5;
				statusControl->setTargetTemperature(targetTemp);
				hotButtonDown = true;
			}
		}
	}
	else {
		hotButtonDown = false;
	}
	firstRun = false;
	delay(2);
}

