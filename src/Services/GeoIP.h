#ifndef GEOIP_H
#define GEOIP_H

#include <Arduino.h>
#include <WiFiClient.h>

class GeoIP {
private:
    bool _hasLocation = false;
    String _longitude;
    String _latitude;
    String _country;
    String _city;
    WiFiClient client;
public:
    const bool &hasLocation;
    const String &latitude;
    const String &longitude;
    const String &country;
    const String &city;

    GeoIP();
    void getLocation();
};

#endif