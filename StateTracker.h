#pragma once
class StateTracker
{
private: 
	int currentState;
	int secondsPassed;
	float temperature;
	float targetTemperature;

public:
	StateTracker();
	~StateTracker();

	int getState();
	void setTemperature(int temperature);
	void setSecoundsPassed(int seconds);
	void setTargetTemperature(float maxTemp);
};

