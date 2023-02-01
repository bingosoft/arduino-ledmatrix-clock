#include "Weather.h"
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "utils.h"

// curl "http://api.openweathermap.org/data/2.5/weather?lat=59.8944&lon=30.2642&appid=9542bcaedac315a65ddec8b335c4290b"
// curl http://freegeoip.net/json/

Weather::Weather() :
	temperature(_temperature),
	city(_city)
{ }

void Weather::setLocation(const String &latitude, const String &longitude, const String &city) {
	this->latitude = latitude;
	this->longitude = longitude;
	_city = city;
}

void Weather::update()
{
	lastUpdate = millis();
	Serial.println("Update weather for the current location");

	HTTPClient http;
	String url = "http://api.openweathermap.org/data/2.5/weather?lat=" + latitude + "&lon=" + longitude + "&appid=" + appId + "&lang=ru";
	Serial.println(url);
	http.begin(_client, url);

	int responseCode = http.GET();

	if (responseCode == HTTP_CODE_OK) {
		String response = http.getString();
		Serial.println(response);

		const size_t BUFFER_SIZE = 1024;
		DynamicJsonBuffer jsonBuffer(BUFFER_SIZE);
		JsonObject& root = jsonBuffer.parseObject(response);
		float t = root["main"]["temp"];
		_temperature = t - 273.15;
		Serial.printf("Current temperature %f\n", t);
	 	_description = (const char *)root["weather"][0]["description"];
		Serial.println("Weather description - " + _description);
	} else {
		Serial.printf("Received bad HTTP response code %d\n", responseCode);
	}
	http.end();
}
