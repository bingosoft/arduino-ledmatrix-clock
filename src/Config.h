#include <Arduino.h>

struct Config {
    String wifiNetwork;
    String wifiPassword;
    int dataInPin = 13;
    int clkPin = 14;
    int csPin = 15;

    Config() {
        wifiNetwork = "bingosoft-wifi";
        wifiPassword = "159753Bingo";
    }
};