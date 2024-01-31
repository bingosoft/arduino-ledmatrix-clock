#include <Arduino.h>
#include <DHT20.h>

class IThermometer {
public:
    bool isConnected;

    virtual float getTemperature() = 0;
    virtual float getHumidity() = 0;
};

class DHT20Thermometer: public IThermometer {
    DHT20 dht;
    bool _isConnected;

    void read();
public:
    const bool &isConnected;

    DHT20Thermometer();

    float getTemperature();
    float getHumidity();
};