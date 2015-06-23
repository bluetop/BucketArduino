#include "DisplayLCD.h"

DisplayLCD::DisplayLCD()
{
	//Old nokia5110 screen start
	//const int sclkPin = 7;  // SCLK or CLK- Serial clock, (10k)
	//const int sdinPin = 6;  // DN or DIN (MOSI) - Serial data, (10k)
	//const int dcPin = 5;    // DC or D/C - Data/Command, (10k)
	//const int scePin = 4;   // SCE or CE or CS - Chip select, (1k) 
	//const int rstPin = 8;   // RST - Reset, pin (10k)
	//const int blPin = 9;    // LED - Backlight LED (330)
	//Adafruit_PCD8544 display = Adafruit_PCD8544(sclkPin, sdinPin, dcPin, scePin, rstPin);
	//display.begin();
	//display.clearDisplay();

	Serial.println("init display...");
	display = new Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
	
	analogWrite(TFT_BACKLIGHT, 150); //TFT lcd backlight pwm value
	display->initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
	display->fillScreen(ST7735_BLACK);
	display->setTextColor(ST7735_WHITE);
	display->setTextWrap(false);
	display->fillScreen(ST7735_BLACK);

	prevState = "";
	prevBeerTemperature = 1000;
	prevTargetTemperature = 1000;
	prevAmbientTemperature = 1000;
	Serial.println("Init screen  done.");
}


DisplayLCD::~DisplayLCD()
{
}

void DisplayLCD::write(char *text, uint16_t color) {
	this->display->setTextSize(1);
	this->display->setCursor(0, 80);
	this->display->setTextColor(color);
	this->display->setTextWrap(true);
	this->display->print(text);
}

void DisplayLCD::writeState(String state, float beerTemperature, float targetTemperature, float ambientTemperature) {
	
	//print temps
	if (beerTemperature != prevBeerTemperature || targetTemperature != prevTargetTemperature) {
		display->setTextColor(ST7735_WHITE);
		display->setTextSize(2);
		display->setCursor(0, 0);
		display->fillRect(27, 0, 92, 52, ST7735_BLACK); //clear temps

		display->print("B: ");
		display->println(beerTemperature);
		display->print("T: ");
		display->println(targetTemperature);
		display->print("A: ");
		display->println(ambientTemperature);

		prevBeerTemperature = beerTemperature;
		prevTargetTemperature = targetTemperature;

	}

	//print status
	if (!state.equals(prevState)) {
		String heating = "HEATING";
		String cooling = "COOLING";

		display->setTextColor(ST7735_WHITE);
		display->setTextSize(2);
		display->setCursor(0, 55);
		display->fillRect(7, 55, 105, 15, ST7735_BLACK); //clear state
		if (cooling.equals(state)) {
			display->setTextColor(ST7735_BLUE);
		}
		else if (heating.equals(state)) {
			display->setTextColor(ST7735_RED);
		}
		else {
			display->setTextColor(ST7735_WHITE);
		}
		display->print("* ");
		display->println(state);
		prevState = state;

		//print key
//		display->setTextColor(ST7735_WHITE);
//		display->setTextSize(1);
//		display->println("");
//		display->print("B=Beer");
//		display->println("  C");
//		display->drawCircle(46, 81, 1.8, ST7735_WHITE);  //degree circle
//		display->println("T=Target temp");
//		display->println("A=Ambient");
	}

	//Serial.println("Finished updating display");
}