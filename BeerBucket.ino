
#include <OneWire.h>
#include <DallasTemperature.h>
#include "TimeTracker.h"
#include "StateController.h"


#define ONE_WIRE_BUS 11

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

//tracks time between state changes
TimeTracker tracker;
StateController statusControl(&tracker);

uint32_t millisSinceLastOp = 0;

unsigned long timeMillis;

void setup(void)
{
	// start serial port
	Serial.begin(9600);
	Serial.println("Dallas Temperature IC Control Library Demo");

	// Start up the library
	sensors.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement

	statusControl.setTargetTemperature(19.0);
}

void loop(void)
{
	// call sensors.requestTemperatures() to issue a global temperature
	// request to all devices on the bus
	Serial.print("Requesting temperatures...");
	sensors.requestTemperatures(); // Send the command to get temperatures
	Serial.println("DONE");
	Serial.print("Temperature for Device 1 is: ");
	float currentTemperature = sensors.getTempCByIndex(0);

	Serial.print(currentTemperature); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
	Serial.println("");


	delay(500);
	Serial.print("Seconds passed... : ");
	Serial.print(tracker.getSeconds());
	Serial.println("");


}



