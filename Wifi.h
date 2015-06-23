#include "Arduino.h"
#include <SoftwareSerial.h>


#define WIFI_TX 3
#define WIFI_RX 4

#pragma once
class Wifi
{
public:
	int static const DISCONNECTED = 10; 
	int static const CONNECTED = 12;
	int static const UNKNOWN = 15;
	int currentState;

	Wifi();
	~Wifi();
	int getState();
	void reset();
	void connect();
	String sendData(String state, float currentTemperature, float targetTemperature, float ambientTemperature);

private: 
	SoftwareSerial* softSerial = new SoftwareSerial(WIFI_RX, WIFI_TX);

	String runCmd(String command);
	void echoToSerial();

};

