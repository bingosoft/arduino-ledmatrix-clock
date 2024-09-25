#include "Thermometer.h"

DHT20Thermometer::DHT20Thermometer()
    : dht(),
    isConnected(_isConnected)
{
    Serial.println("[DHT20] init thermometer...");
    _isConnected = dht.begin(PIN_WIRE_SDA, PIN_WIRE_SCL);

    Serial.printf("[DHT20] DHT20 thermometer is connected - %d\n", _isConnected);

    if (isConnected) {
        read();
		Serial.println("[DHT20] Room thermometer DHT20 is connected");
    } else {
		Serial.println("[DHT20] Room thermometer feature is disabled - device is not connected");
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
    Serial.println("[DHT20] reading latest values...");
    int status = dht.read();

    switch (status) {
    case DHT20_OK:
        Serial.println("[DHT20] Read status: OK");
        break;
    case DHT20_ERROR_CHECKSUM:
        Serial.println("[DHT20] Read status: Checksum error");
        break;
    case DHT20_ERROR_CONNECT:
        Serial.println("[DHT20] Read status: Connect error");
        break;
    case DHT20_MISSING_BYTES:
        Serial.println("[DHT20] Read status: Missing bytes");
        break;
    case DHT20_ERROR_BYTES_ALL_ZERO:
        Serial.println("[DHT20] Read status: All bytes read zero");
        break;
    case DHT20_ERROR_READ_TIMEOUT:
        Serial.println("[DHT20] Read status: Read time out");
        break;
    case DHT20_ERROR_LASTREAD:
        Serial.println("[DHT20] Read status: Error read too fast");
        break;
    default:
        Serial.println("[DHT20] Read status: Unknown error");
        break;
    }

    _isConnected = status == DHT20_OK;
}