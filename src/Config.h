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

    Config(
        String wifiNetwork,
        String wifiPassword,
        String ntpServerIP,
        int dataInPin,
        int clkPin,
        int csPin
    ) : wifiNetwork(wifiNetwork),
        wifiPassword(wifiPassword),
        ntpServerIP(ntpServerIP),
        dataInPin(dataInPin),
        clkPin(clkPin),
        csPin(csPin)
    { }

    Config() { }
};

#endif