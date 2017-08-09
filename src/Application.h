#ifndef APPLICATION_H
#define APPLICATION_H

#include <LedMatrix.h>
#include <Arduino.h>
#include <NTPClient.h>
#include <Timers.h>
#include "Weather.h"

class Application
{
	// const char* ssid = "network55";
	// const char* password = "eiS4aepheehi";
	const char* ssid = "bingosoft-wifi";
	const char* password = "159753bingo";

	// GPIO digital pins
	const int DATA_IN = 13;
	const int CS = 15;
	const int CLK = 14;

	Weather w;
	LedMatrix l;
	NTPClient ntp;
	Timers<4> t;
	bool dotVisible = false;

	void connectToWiFi();
	void displayTime();
	void displayWeather();
	void displayDescription();

	void updateWeather() { w.update(); }
	void updateTime() { ntp.update(); }
public:
	Application();
	void exec();
};

#endif
