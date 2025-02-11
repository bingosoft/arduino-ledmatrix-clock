#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

namespace App {
    struct Config {
        uint16_t weatherDataUpdateIntervalSeconds;
        uint16_t timeUpdateIntervalSeconds;
        uint16_t floatingTextMovingDelay;
        uint16_t floatingTextStartDelay;
        uint16_t dataInPin;
        uint16_t clkPin;
        uint16_t csPin;
        String wifiNetwork;
        String wifiPassword;
        std::optional<String> overrideLatitude;
        std::optional<String> overrideLongitude;

        static Config current;

        Config() :
            weatherDataUpdateIntervalSeconds(),
            timeUpdateIntervalSeconds(),
            dataInPin(13),
            clkPin(14),
            csPin(15),
            wifiNetwork(),
            wifiPassword()
        { }
    };
}

#endif