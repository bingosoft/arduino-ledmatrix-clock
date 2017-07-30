#ifndef APPLICATION_H
#define APPLICATION_H

#include <LedMatrix.h>
#include <Arduino.h>
#include <NTPClient.h>

class Application
{
	const char* ssid = "network55";
	const char* password = "eiS4aepheehi";

	// GPIO digital pins
	const int DATA_IN = 13;
	const int CS = 15;
	const int CLK = 14;

	LedMatrix l;
	NTPClient ntp;
	bool dotVisible = false;

	void updateTime();
public:
	Application();
	void exec();
};

#endif
