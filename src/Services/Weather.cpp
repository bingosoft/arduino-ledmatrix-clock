#include "Weather.h"
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// curl "http://api.openweathermap.org/data/2.5/weather?lat=59.8944&lon=30.2642&appid=9542bcaedac315a65ddec8b335c4290b"
// curl http://freegeoip.net/json/

Weather::Weather() :
	temperature(_temperature),
	windSpeed(_windSpeed),
	humidity(_humidity),
	windDirectionAngle(_windDirectionAngle),
	timezoneSeconds(_timezoneSeconds),
	city(_city),
	description(_description),
	weatherDiffDirection(_weatherDiffDirection),
	delegate()
{ }

void Weather::setLocation(const String &latitude, const String &longitude) {
	this->latitude = latitude;
	this->longitude = longitude;
}

void Weather::update() {
	Serial.println("[Weather] Update weather for the current location");

	HTTPClient http;
	String url = "http://api.openweathermap.org/data/2.5/weather?lat=" + latitude + "&lon=" + longitude + "&appid=" + appId + "&lang=ru&units=metric";
	http.begin(_client, url);
	int responseCode = http.GET();
    String response = http.getString();
    http.end();

	if (responseCode != HTTP_CODE_OK) {
		Serial.printf("[Weather] Received bad HTTP response code %d\n", responseCode);
        return;
    }

    Serial.println("[Weather] Response: " + response);

    JsonDocument root;
    DeserializationError error = deserializeJson(root, response);

    if (error) {
        Serial.printf("[Weather] JSON deserialization error - %s\n", error.c_str());
        return;
    }

    float newTemperature = root["main"]["temp"];

    if (lastUpdate > 0) {
        Serial.printf("[Weather] Previous temperature %.1f, new - %.1f\n", _temperature, newTemperature);

        if (newTemperature > _temperature) {
            weatherDiffDirection = WeatherTemperatureDiffDirection::raising;
        } else if (newTemperature < _temperature) {
            weatherDiffDirection = WeatherTemperatureDiffDirection::falling;
        } else {
            weatherDiffDirection = WeatherTemperatureDiffDirection::notChanged;
        }
    }

    _temperature = newTemperature;
    _humidity = root["main"]["humidity"];
    _windSpeed = root["wind"]["speed"];
    _windDirectionAngle = root["wind"]["deg"];
    _timezoneSeconds = root["timezone"];
    _description = (const char *)root["weather"][0]["description"];
    _city = (const char *)root["name"];
    Serial.printf("[Weather] Current temperature %.1f\n", _temperature);
    Serial.println("[Weather] Weather description - " + _description);

    if (delegate) {
        delegate->onWeatherUpdated();
    }

    lastUpdate = millis();
}
