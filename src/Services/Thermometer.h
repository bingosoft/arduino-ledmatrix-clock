#include <Arduino.h>
#include <DHT20.h>

class IThermometer {
public:
    virtual bool isConnected() const = 0;
    virtual float getTemperature() = 0;
    virtual float getHumidity() = 0;
};

class DHT20Thermometer: public IThermometer {
    DHT20 dht;
    bool _isConnected;

    void read();
public:
    DHT20Thermometer();

    bool isConnected() const;
    float getTemperature();
    float getHumidity();
};