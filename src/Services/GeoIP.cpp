#include "GeoIP.h"
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

GeoIP::GeoIP() :
	hasLocation(_hasLocation),
    latitude(_latitude),
    longitude(_longitude),
	country(_country),
    city(_city)
{ }

void GeoIP::getLocation() {
	Serial.println("[GeoIP] Getting current location by IP...");

	HTTPClient http;
	http.begin(client, "http://ip-api.com/json/");
	int responseCode = http.GET();

	if (responseCode != HTTP_CODE_OK) {
		Serial.printf("[GeoIP] Invalid response code from GeoIP - %d", responseCode);
        http.end();
    }

    String response = http.getString();
    http.end();
    Serial.println("[GeoIP] Response: " + response);

    JsonDocument root;
    DeserializationError error = deserializeJson(root, response);

    if (error) {
        Serial.printf("[GeoIP] JSON deserialization error - %s\n", error.c_str());
        return;
    }

    String status = (const char *)root["status"];

    if (status != "success") {
        Serial.println("[GeoIP] GeoIP status is not success");
        return;
    }

    char s[15];
    double lat = root["lat"];
    dtostrf(lat, 0, 4, s);
    _latitude = String(s);
    double lon = root["lon"];
    dtostrf(lon, 0, 4, s);
    _longitude = String(s);
    _country = (const char *)root["country"];
    _city = (const char *)root["city"];
    Serial.printf("[GeoIP] Location by IP received - Lat: %s, Lon: %s, City: %s\n", latitude.c_str(), longitude.c_str(), city.c_str());
    _hasLocation = true;
}

