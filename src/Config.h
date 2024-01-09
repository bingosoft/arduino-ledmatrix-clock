#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

namespace App {
    struct Config {
        u16 weatherDataUpdateIntervalSeconds;
        u16 timeUpdateIntervalSeconds;
        u16 floatingTextMovingDelay;
        u16 floatingTextStartDelay;
        int dataInPin;
        int clkPin;
        int csPin;
        bool hasThermometerSensor;
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
            hasThermometerSensor(false),
            wifiNetwork(),
            wifiPassword(),
            ntpServerIP()
        { }
    };
}

#endif