#include "Config.h"
#include <string_view>

#define WIFI_SSID "WIFI_SSID_NAME"
#define WIFI_PASSWORD "WIFI_PASSWORD"

namespace App {
    static Config makeDefaultConfig() {
        Config config = Config();

        static_assert(std::string_view(WIFI_SSID) != "WIFI_SSID_NAME", "Please define your WiFi SSID and password in Config.cpp:4");
        static_assert(std::string_view(WIFI_PASSWORD) != "WIFI_PASSWORD", "Please define your WiFi SSID and password in Config.cpp:5");

        config.floatingTextMovingDelay = 30;
        config.floatingTextStartDelay = 700;
        config.wifiNetwork = WIFI_SSID;
        config.wifiPassword = WIFI_PASSWORD;
        config.weatherDataUpdateIntervalSeconds = 3 * 60;
        config.timeUpdateIntervalSeconds = 30 * 60;

        // If calculated GeoIP location is not precise you can override your location

        // config.overrideLatitude = "55.485576";
        // config.overrideLongitude = "28.768349";

        return config;
    }

    Config Config::current = makeDefaultConfig();
}