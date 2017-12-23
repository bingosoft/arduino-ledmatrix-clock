#include "Weather.h"
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// curl "http://api.openweathermap.org/data/2.5/weather?lat=59.8944&lon=30.2642&appid=9542bcaedac315a65ddec8b335c4290b"
// curl http://freegeoip.net/json/


void Weather::getLocation()
{
	HTTPClient http;
	http.begin("http://freegeoip.net/json/");
	if (http.GET() == HTTP_CODE_OK) {
		String response = http.getString();
		Serial.println(response);

		const size_t BUFFER_SIZE = 512;
		DynamicJsonBuffer jsonBuffer(BUFFER_SIZE);
		JsonObject& root = jsonBuffer.parseObject(response);
		char s[15];
		double lat = root["latitude"];
		dtostrf(lat, 0, 4, s);
		latitude = String(s);
		double lon = root["longitude"];
		dtostrf(lon, 0, 4, s);
		longitude = String(s);
		_city = (const char *)root["city"];
		_city.toLowerCase();
		Serial.println("Location by IP received - " + latitude + " " + longitude);
	}
	http.end();
}

void Weather::update()
{
	lastUpdate = millis();
	Serial.println("Update weather for the current location");

	HTTPClient http;
	String url = "http://api.openweathermap.org/data/2.5/weather?lat=" + latitude + "&lon=" + longitude + "&appid=9542bcaedac315a65ddec8b335c4290b&lang=ru";
	http.begin(url);

	if (http.GET() == HTTP_CODE_OK) {
		String response = http.getString();
		Serial.println(response);

		const size_t BUFFER_SIZE = 1024;
		DynamicJsonBuffer jsonBuffer(BUFFER_SIZE);
		JsonObject& root = jsonBuffer.parseObject(response);
		float t = root["main"]["temp"];
		_temperature = t - 273.15;
		Serial.printf("Current temperature %d\n", t);
	 	_description = (const char *)root["weather"][0]["description"];
		Serial.println("Weather description - " + _description);
	}
	http.end();
}
