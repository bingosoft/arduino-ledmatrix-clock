#include "Config.h"
#include <string_view>

#define WIFI_SSID "WIFI_SSID_NAME"
#define WIFI_PASSWORD "WIFI_PASSWORD"

Config makeDefaultConfig() {
    Config config = Config();

    static_assert(std::string_view(WIFI_SSID) != "WIFI_SSID_NAME", "Please define your WiFi SSID and password in Config.cpp:3");
    static_assert(std::string_view(WIFI_PASSWORD) != "WIFI_PASSWORD", "Please define your WiFi SSID and password in Config.cpp:4");

    config.wifiNetwork = WIFI_SSID;
    config.wifiPassword = WIFI_PASSWORD;
    config.ntpServerIP = "142.132.210.78";

    return config;
}

Config Config::d1Mini = makeDefaultConfig();