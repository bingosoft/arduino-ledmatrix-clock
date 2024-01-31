#include "Thermometer.h"

DHT20Thermometer::DHT20Thermometer()
    : isConnected(_isConnected)
{
    _isConnected = dht.begin();

    Serial.printf("DHT20 thermometer is connected - %d\n", _isConnected);

    if (isConnected) {
        read();
		Serial.println("Room thermometer DHT20 is connected");
    } else {
		Serial.println("Room thermometer feature is disabled - device is not connected");
    }
}

float DHT20Thermometer::getTemperature()
{
    read();
    return dht.getTemperature();
}

float DHT20Thermometer::getHumidity()
{
    read();
    return dht.getHumidity();
}

void DHT20Thermometer::read()
{
    int status = dht.read();

    switch (status) {
    case DHT20_OK:
        Serial.println("DHT - OK");
        break;
    case DHT20_ERROR_CHECKSUM:
        Serial.println("DHT - Checksum error");
        break;
    case DHT20_ERROR_CONNECT:
        Serial.println("DHT - Connect error");
        break;
    case DHT20_MISSING_BYTES:
        Serial.println("DHT - Missing bytes");
        break;
    case DHT20_ERROR_BYTES_ALL_ZERO:
        Serial.println("DHT - All bytes read zero");
        break;
    case DHT20_ERROR_READ_TIMEOUT:
        Serial.println("DHT - Read time out");
        break;
    case DHT20_ERROR_LASTREAD:
        Serial.println("DHT - Error read too fast");
        break;
    default:
        Serial.println("DHT - Unknown error");
        break;
    }

    _isConnected = status == DHT20_OK;
}