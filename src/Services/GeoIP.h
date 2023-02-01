#ifndef GEOIP_H
#define GEOIP_H

#include <Arduino.h>
#include <WiFiClient.h>

class GeoIP {
private:
    WiFiClient client;
    String _longitude;
    String _latitude;
    String _city;
public:
    const String &latitude;
    const String &longitude;
    const String &city;

    GeoIP();
    void getLocation();
};

#endif