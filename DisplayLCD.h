#pragma once

#include "Arduino.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h> // Hardware-specific library

//#include <Adafruit_PCD8544.h>


class DisplayLCD
{
private:

//	const int sclkPin = 7;  // SCLK or CLK- Serial clock,
//	const int sdinPin = 6;  // DN or DIN (MOSI) - Serial data,
//	const int dcPin = 5;    // DC or D/C - Data/Command,
//	const int scePin = 4;   // SCE or CE or CS - Chip select, 
//	const int rstPin = 8;   // RST - Reset, pin 
//	const int blPin = 9;    // LED - Backlight LED
//	Adafruit_PCD8544 display = Adafruit_PCD8544(sclkPin, sdinPin, dcPin, scePin, rstPin);

#define TFT_BACKLIGHT 5
	// For the breakout, you can use any 2 or 3 pins
	// These pins will also work for the 1.8" TFT shield
#define TFT_CS     8
#define TFT_RST    9  // you can also connect this to the Arduino reset
	// in which case, set this #define pin to 0!
#define TFT_DC     7
	// Option 1 (recommended): must use the hardware SPI pins
	// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
	// an output. This is much faster - also required if you want
	// to use the microSD card (see the image drawing example)
	Adafruit_ST7735* display; 
	// Option 2: use any pins but a little slower!
#define TFT_SCLK 13   // set these to be whatever pins you like!
#define TFT_MOSI 11   // set these to be whatever pins you like!
	//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

	String prevState;
	float prevBeerTemperature;
	float prevTargetTemperature;
	float prevAmbientTemperature;

public:
	DisplayLCD();
	~DisplayLCD();

	void write(char *text);
	void write(char *text, uint16_t color);
	void writeState(String state, float beerTemperature, float targetTemperature, float ambientTemperature);
};
