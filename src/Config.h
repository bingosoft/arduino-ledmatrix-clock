#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

struct Config {
    u16 weatherDataUpdateIntervalSeconds;
    u16 timeUpdateIntervalSeconds;
    int dataInPin;
    int clkPin;
    int csPin;
    String wifiNetwork;
    String wifiPassword;
    String ntpServerIP;
    std::optional<String> overrideLatitude;
    std::optional<String> overrideLongitude;

    static Config d1Mini;

    Config() :
        weatherDataUpdateIntervalSeconds(),
        timeUpdateIntervalSeconds(),
        dataInPin(13),
        clkPin(14),
        csPin(15),
        wifiNetwork(),
        wifiPassword(),
        ntpServerIP()
    { }
};

#endif