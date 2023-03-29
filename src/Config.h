#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

struct Config {
    String wifiNetwork;
    String wifiPassword;
    String ntpServerIP;
    int dataInPin;
    int clkPin;
    int csPin;

    static Config d1Mini;

    Config() :
        wifiNetwork(),
        wifiPassword(),
        ntpServerIP(),
        dataInPin(13),
        clkPin(14),
        csPin(15)
    { }
};

#endif