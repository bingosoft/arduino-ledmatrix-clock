#include "Application.h"
#include <ESP8266WiFi.h>
#include "Utils/Formatter.h"

Application::Application(const App::Config &config) :
    thermometer(),
	ledmatrix(config.dataInPin, config.clkPin, config.csPin),
	ntpClient(),
	config(config)
{
	Serial.println("[App] Init app...");

    ledmatrix.setIntensity(0);
	connectToWiFi();

	if (!getTime()) {
        ESP.restart();
		return;
	}

	ledmatrix.clearDisplay();
	weather.delegate = this;

	if (config.overrideLatitude && config.overrideLongitude) {
		Serial.println("Having an overriden location in config, skipping GeoIP...");
		weather.setLocation(*config.overrideLatitude, *config.overrideLongitude);
	} else if (getLocation()) {
		weather.setLocation(geoIP.latitude, geoIP.longitude);
	}

	weather.update();
	subscribeTimers();
}

void Application::subscribeTimers() {
    Serial.println("[App] Subscribing timers...");
	timers.schedule(15 * 1000, this, &Application::displayTemperature, 5000);

	if (thermometer.isConnected) {
		timers.schedule(30 * 1000, this, &Application::displayTemperatureInRoom, 10000);
	} else {
        Serial.println("[App] Room thermometer was not found. Disabling room temperature feature.");
    }

	timers.schedule(30 * 1000, this, &Application::displayDescription, 15000);
	timers.schedule(60 * 1000, this, &Application::displayHumidity, 20000);

	if (thermometer.isConnected) {
		timers.schedule(60 * 1000, this, &Application::displayHumidityInRoom, 35000);
	}

	timers.schedule(5 * 60 * 1000, this, &Application::displayCity, 30000);
	timers.schedule(120 * 1000, this, &Application::displayWind, 50000);
	timers.schedule(config.weatherDataUpdateIntervalSeconds * 1000, &weather, &Weather::update);
	timers.schedule(config.timeUpdateIntervalSeconds * 1000, &ntpClient, &NTPClient::getTime);
}

bool Application::getTime() {
	Serial.println("[App] Getting time...");
	ledmatrix.renderFloatingText("Getting time...", config.floatingTextStartDelay, config.floatingTextMovingDelay);
	ledmatrix.clearDisplay();

	const int maxRetries = 4;
	int retries = 0;

	ledmatrix.turnLed(0, 1, true);
	ntpClient.getTime();

	while (!ntpClient.hasTime() && retries < maxRetries) {
		delay(500);
		ledmatrix.turnLed(retries + 1, 1, true);
		retries++;
		Serial.printf("NTP time still not received. Retry again. Retries: %d\n", retries);
		ntpClient.getTime();
	}

	if (!ntpClient.hasTime()) {
		ledmatrix.renderFloatingText("NTP error", 5000, config.floatingTextMovingDelay);
	}

	return ntpClient.hasTime();
}

bool Application::getLocation() {
	Serial.println("[App] Getting location...");
	ledmatrix.renderFloatingText("Getting location...", config.floatingTextStartDelay, config.floatingTextMovingDelay);
	geoIP.getLocation();
	
	if (geoIP.hasLocation) {
		ledmatrix.renderFloatingText(geoIP.country, config.floatingTextStartDelay, config.floatingTextMovingDelay);
		ledmatrix.clearDisplay();
		ledmatrix.renderFloatingText(geoIP.city, config.floatingTextStartDelay, config.floatingTextMovingDelay);
		ledmatrix.clearDisplay();
	} else {
		ledmatrix.renderFloatingText("Error getting location", config.floatingTextStartDelay, config.floatingTextMovingDelay);
	}

	return geoIP.hasLocation;
}

void Application::connectToWiFi()
{
	Serial.printf("[App] Connecting to Wi-Fi network %s, password %s\n", config.wifiNetwork.c_str(), config.wifiPassword.c_str());

	ledmatrix.renderFloatingText("Connecting...", config.floatingTextStartDelay, config.floatingTextMovingDelay);
	ledmatrix.renderFloatingText(config.wifiNetwork, config.floatingTextStartDelay, config.floatingTextMovingDelay);
	ledmatrix.clearDisplay();

	int i = 0;
	WiFi.begin(config.wifiNetwork, config.wifiPassword);

	while (!WiFi.isConnected()) {
		ledmatrix.turnLed(i % 32, i / 32, true);
		++i;
		Serial.print(".");
		if (i >= 16) {
			Serial.printf("\n[App] No response from access point\n");
			i = 0;
			WiFi.disconnect();
			WiFi.begin(config.wifiNetwork, config.wifiPassword);
			ledmatrix.clearDisplay();
		}
		delay(500);
	}

	Serial.printf("\n[App] Connected to Wi-Fi - %s\n", WiFi.localIP().toString().c_str());
	ledmatrix.clearDisplay();
}

void Application::displayTime()
{
	ledmatrix.turnLed(16, 7, dotVisible);
	ledmatrix.turnLed(15, 7, dotVisible);
	dotVisible = !dotVisible;

	int hours = ntpClient.hours();
	int minutes = ntpClient.minutes();
	int seconds = ntpClient.seconds();
	ledmatrix.renderChar(hours / 10 + 0x30, 2);
	ledmatrix.renderChar(hours % 10 + 0x30, 9);
	ledmatrix.renderChar(minutes / 10 + 0x30, 18);
	ledmatrix.renderChar(minutes % 10 + 0x30, 25);

    if (dotVisible)
        Serial.printf("[App] Displaying time - %02d:%02d:%02d\n", hours, minutes, seconds);
}

void Application::displayTemperature()
{
	String temperatureDiffSign = "";

	switch (weather.weatherDiffDirection) {
	case Weather::WeatherTemperatureDiffDirection::raising:
		temperatureDiffSign = "↑";
		break;
	case Weather::WeatherTemperatureDiffDirection::falling:
		temperatureDiffSign = "↓";
		break;
	default:
		break;
	}

	String temperature = temperatureDiffSign + (weather.temperature > 0 ? "+" : weather.temperature < 0 ? "-" : "") + Formatter::format("%.1f", std::abs(weather.temperature)) + "°";
	Serial.printf("[App] Displaying temperature - %s\n", temperature.c_str());

	ledmatrix.clearDisplay();
	ledmatrix.renderStringInCenter(temperature, 3000);
	ledmatrix.clearDisplay();
}

void Application::displayTemperatureInRoom()
{
	float temp = thermometer.getTemperature();
	String temperature = String("внутри ") + (temp > 0 ? "+" : temp < 0 ? "-" : "") + Formatter::format("%.1f", std::abs(temp)) + "°";
	Serial.printf("[App] Displaying temperature in room - %s\n", temperature.c_str());

	ledmatrix.clearDisplay();
	ledmatrix.renderFloatingText(temperature, config.floatingTextStartDelay, config.floatingTextMovingDelay);
	ledmatrix.clearDisplay();
}

void Application::displayCity()
{
	Serial.printf("Displaying city %s\n", weather.city.c_str());
	ledmatrix.clearDisplay();
	ledmatrix.renderFloatingText(weather.city, config.floatingTextStartDelay, config.floatingTextMovingDelay);
	ledmatrix.clearDisplay();
}

void Application::displayDescription()
{
	Serial.printf("[App] Displaying weather description %s\n", weather.description.c_str());
	ledmatrix.clearDisplay();
	ledmatrix.renderFloatingText(weather.description, config.floatingTextStartDelay, config.floatingTextMovingDelay);
	ledmatrix.clearDisplay();
}

void Application::displayHumidity()
{
	String humidity = "влажность " + String(weather.humidity) + "%";
	Serial.printf("[App] Displaying humidity - %s\n", humidity.c_str());

	ledmatrix.clearDisplay();
	ledmatrix.renderFloatingText(humidity, config.floatingTextStartDelay, config.floatingTextMovingDelay);
	ledmatrix.clearDisplay();
}

void Application::displayHumidityInRoom()
{
	String humidity = "влaжн. внутри " + String(std::lround(thermometer.getHumidity())) + "%";
	Serial.printf("[App] Displaying humidity inside room - %s\n", humidity.c_str());

	ledmatrix.clearDisplay();
	ledmatrix.renderFloatingText(humidity, config.floatingTextStartDelay, config.floatingTextMovingDelay);
	ledmatrix.clearDisplay();
}

void Application::displayWind()
{
	String directions[] = {"северный", "северо-восточный", "восточный", "юго-восточный", "южный", "юго-западный", "западный", "северо-западный"};
	int index = std::lround(weather.windDirectionAngle * 8 / 360.0) % 8;
	String direction = directions[index];
	String windSpeed = Formatter::format("%.1f", weather.windSpeed);
	String wind = "ветер " + direction + " " + windSpeed + " м/с";
	Serial.printf("[App] Displaying wind - %s\n", wind.c_str());

	ledmatrix.clearDisplay();
	ledmatrix.renderFloatingText(wind, config.floatingTextStartDelay, config.floatingTextMovingDelay);
	ledmatrix.clearDisplay();
}

void Application::exec()
{
	if (ntpClient.hasTime()) {
		timers.tick();
		displayTime();
	}
	delay(500);
}

// WeatherDelegate

void Application::onWeatherUpdated()
{
	Serial.println("[App] Weather updated");
	ntpClient.setTimeZone(weather.timezoneSeconds);
}