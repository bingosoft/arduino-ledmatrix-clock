#ifndef APPLICATION_H
#define APPLICATION_H

#include <LedMatrix.h>
#include <Arduino.h>
#include <NTPClient.h>
#include <Timers.h>
#include "Config.h"
#include "Services/Weather.h"
#include "Services/GeoIP.h"

class Application: WeatherDelegate
{
	const int messagesScrollDelay = 30;
	Weather weather;
	GeoIP geoIP;
	LedMatrix ledmatrix;
	NTPClient ntpClient;
	Timers<7> timers;
	Config config;
	bool dotVisible = false;

	void subscribeTimers();
	void connectToWiFi();
	bool getTime();
	bool getLocation();
	void displayTime();
	void displayTemperature();
	void displayCity();
	void displayDescription();
	void displayHumidity();
	void displayWind();

	// WeatherDelegate methods
	void onWeatherUpdated();

public:
	Application(const Config &config);
	void exec();
};

#endif
