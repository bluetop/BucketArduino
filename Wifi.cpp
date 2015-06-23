#include "Wifi.h"


#define RESET_PIN A3
#define RESPONSE_TIMEOUT 6000
#define SSID        "ITEAD"
#define PASSWORD    "12345678"
#define HOST_NAME   "www.baidu.com"
#define HOST_PORT   (80)


Wifi::Wifi()
{
	//SoftwareSerial softSerial(WIFI_RX, WIFI_TX); /* RX:D3, TX:D2 */
	//this->softSerial = new SoftwareSerial (WIFI_RX, WIFI_TX); /* RX:D3, TX:D2 */

	pinMode(RESET_PIN, OUTPUT);
	digitalWrite(RESET_PIN, HIGH);
	
	//reset();
	//echoToSerial();
	//delay(100);
	//echoToSerial();
}


Wifi::~Wifi()
{
}

int Wifi::getState() {
	return this->UNKNOWN;
}


void Wifi::connect() {
	//runCmd("AT+CWMODE=1");
	//runCmd("");
	//echoToSerial();
	//runCmd("AT+RST");
	//delay(2000);
	//echoToSerial();
	//runCmd("AT+CWMODE=2");
	//runCmd("AT+CWLAP");
	//delay(2000);
	//echoToSerial();
	//String cmd = "AT+CWJAP=\"";
	//cmd += SSID;
	//cmd += "\",\"";
	//cmd += PASSWORD;
	//cmd += "\"";
	//runCmd(cmd);
	//delay(2000);
	//echoToSerial();
	//if (this->softSerial->find("OK")) {
	//	this->currentState = this->CONNECTED;
	//	Serial.println("Reset wifi OK.");
	//}
	//else {
	//	this->currentState = this->UNKNOWN;
	//}

//	this->softSerial->end();
}


//Runs an AT command
String Wifi::runCmd(String command) {
	String echocmd = "Running: ";
	String response = "";
	command += "\r\n";
	echocmd += command;
	this->softSerial->begin(9600);
	this->softSerial->setTimeout(RESPONSE_TIMEOUT);
	Serial.println(echocmd);
	this->softSerial->print(command);
	//this->softSerial->end();
	//delay(100);
	this->softSerial->begin(9600);
	this->softSerial->setTimeout(RESPONSE_TIMEOUT);
	delay(3000);
	while (this->softSerial->available()) {
		response += this->softSerial->readString();
	}
	this->softSerial->end();


//	for (int i = 0; i < 5; i++) {
//		while (this->softSerial->available()) {
//			response += this->softSerial->readString();
//		}
//		delay(400);
//	}
	//while (this->softSerial->available() || delayTime < 4000)
	//{

	//	String c = this->softSerial->readString();
	//	response += c;
	//	delay(3000);
	//}

	Serial.print("Response: ");
	Serial.println(response);
	Serial.println("----------");
	return response;
	//echoToSerial();
}




void Wifi::echoToSerial()
{
	while (this->softSerial->available())
	{
		char c = this->softSerial->read();
		Serial.write(c);
		if (c == '\r') Serial.print('\n');
	}
	Serial.println("====");
}


void Wifi::reset() {
	Serial.println("Resetting wifi.");
	digitalWrite(RESET_PIN, LOW);
	delay(100);
	digitalWrite(RESET_PIN, HIGH);
}

String Wifi::sendData(String state, float currentTemperature, float targetTemperature, float ambientTemperature) {
	//basic sanity check first
	if (currentTemperature < 0) {
		//return "bad";
		currentTemperature = 22;
	}
	float chillerBoxTemperature = 0;

	
	String command = "send_data('";
	command += currentTemperature;
	command += "','";
	command += chillerBoxTemperature;
	command += "','";
	command += targetTemperature;
	command += "','";
	command += ambientTemperature;
	command += "','";
	command += state;
	command += "')";
	String echocmd = "Running: ";
	String response = "";
	command += "\r\n";
	echocmd += command;
	this->softSerial->begin(9600);
	this->softSerial->setTimeout(RESPONSE_TIMEOUT);
	Serial.println(echocmd);
	this->softSerial->print(command);
	delay(2000);
	String result = this->softSerial->readString();
	this->softSerial->end();
	int start = result.indexOf('>');
	int end = result.indexOf('<');
	if (end != -1 && start != -1) {
		return result.substring(start, end);
	}
	return result;
}
