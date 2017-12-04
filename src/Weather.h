#ifndef WEATHER_H
#define WEATHER_H

#include <Arduino.h>

class Weather
{
	String longitude;
	String latitude;
	int temp;
	String descr;
	unsigned long lastUpdate = 0;

public:
	void getLocation();
	bool locationReceived() const { return longitude.length() > 0; }
	void update();
	int temperature() const { return temp; }
	const String & description() const { return descr; }
	int secondsSinceLastUpdate() const { return (millis() - lastUpdate) / 1000; }
};

#endif
