#ifndef WEATHER_H
#define WEATHER_H

#include <Arduino.h>
#include <WiFiClient.h>

class WeatherDelegate {
public:
	virtual void onWeatherUpdated() = 0;
};

class Weather
{
	float _temperature;
	float _windSpeed;
	int _humidity;
	int _windDirectionAngle;
	int _timezoneSeconds;
	String _city;
	String _description;
	WiFiClient _client;
	String latitude;
	String longitude;
	const String appId = "9542bcaedac315a65ddec8b335c4290b";
	unsigned long lastUpdate = 0;

public:
	const float &temperature;
	const float &windSpeed;
	const int &humidity;
	const int &windDirectionAngle;
	const int &timezoneSeconds;
	const String &city;
	const String &description;

	WeatherDelegate *delegate;

	Weather();
	void setLocation(const String &longitude, const String &latitude);
	void update();
	int secondsSinceLastUpdate() const { return (millis() - lastUpdate) / 1000; }
};

#endif
