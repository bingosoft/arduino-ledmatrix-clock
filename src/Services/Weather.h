#ifndef WEATHER_H
#define WEATHER_H

#include <Arduino.h>
#include <WiFiClient.h>

class Weather
{
	float _temperature;
	int _humidity;
	String _city;
	String _description;
	WiFiClient _client;
	String longitude;
	String latitude;
	const String appId = "9542bcaedac315a65ddec8b335c4290b";
	unsigned long lastUpdate = 0;

public:
	const float &temperature;
	const int &humidity;
	const String &city;
	const String &description;

	Weather();
	void setLocation(const String &longitude, const String &latitude, const String &city);
	void update();
	int secondsSinceLastUpdate() const { return (millis() - lastUpdate) / 1000; }
};

#endif
