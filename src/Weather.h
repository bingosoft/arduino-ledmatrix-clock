#ifndef WEATHER_H
#define WEATHER_H

#include <Arduino.h>

class Weather
{
	String longitude;
	String latitude;
	int _temperature;
	String _description;
	String _city;
	unsigned long lastUpdate = 0;

public:
	void getLocation();
	bool locationReceived() const { return longitude.length() > 0; }
	void update();
	int temperature() const { return _temperature; }
	const String & city() const { return _city; }
	const String & description() const { return _description; }
	int secondsSinceLastUpdate() const { return (millis() - lastUpdate) / 1000; }
};

#endif
