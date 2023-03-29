#include "Application.h"
#include <NTPClient.h>
#include <ESP8266WiFi.h>

Application::Application(const Config &config) :
	ledmatrix(config.dataInPin, config.clkPin, config.csPin),
	ntpClient(NTPClient(config.ntpServerIP)),
	config(config)
{
	Serial.println("Init app...");

    ledmatrix.setIntensity(0);
	// ledmatrix.renderFloatingText("абвгдеёжзиклмнопрстуфхцчшщьыъэюя", 5000, 400);
	// ledmatrix.clearDisplay();
	// ledmatrix.renderFloatingText("abcdefghijklmnopqrstuvwxyz+- ", 5000, 400);
	connectToWiFi();

	if (!getTime()) {
		ESP.reset();
		return;
	}

	if (getLocation()) {
		weather.setLocation(geoIP.latitude, geoIP.longitude, geoIP.city);
		weather.update();
	}

	subscribeTimers();
}

void Application::subscribeTimers() {
	timers.schedule(30 * 1000, this, &Application::displayTemperature, 10000);
	timers.schedule(30 * 1000, this, &Application::displayDescription, 20000);
	timers.schedule(5 * 60 * 1000, this, &Application::displayCity, 30000);
	timers.schedule(config.weatherDataUpdateIntervalSeconds * 1000, &weather, &Weather::update);
	timers.schedule(config.timeUpdateIntervalSeconds * 1000, &ntpClient, &NTPClient::getTime);
}

bool Application::getTime() {
	Serial.println("Getting time...");
	ledmatrix.renderFloatingText("Getting time...", 1000, messagesScrollDelay);
	ledmatrix.clearDisplay();

	const int maxRetries = 4;
	int retries = 0;
	ntpClient.setTimeZone(60 * 60);

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
		ledmatrix.renderFloatingText("NTP error", 5000, messagesScrollDelay);
	}

	return ntpClient.hasTime();
}

bool Application::getLocation() {
	Serial.println("Getting location...");
	ledmatrix.renderFloatingText("Getting location...", 1000, messagesScrollDelay);
	geoIP.getLocation();
	
	if (geoIP.hasLocation) {
		ledmatrix.renderFloatingText(geoIP.country, 1000, messagesScrollDelay);
		ledmatrix.clearDisplay();
		ledmatrix.renderFloatingText(geoIP.city, 1000, messagesScrollDelay);
		ledmatrix.clearDisplay();
	} else {
		ledmatrix.renderFloatingText("Error getting location", 1000, messagesScrollDelay);
	}

	return geoIP.hasLocation;
}

void Application::connectToWiFi()
{
	Serial.printf("Connecting to Wi-Fi network %s, password %s\n", config.wifiNetwork.c_str(), config.wifiPassword.c_str());

	ledmatrix.renderFloatingText("Connecting...", 1000, messagesScrollDelay);
	ledmatrix.renderFloatingText(config.wifiNetwork, 1000, messagesScrollDelay);
	ledmatrix.clearDisplay();

	int i = 0;
	WiFi.begin(config.wifiNetwork, config.wifiPassword);

	while (!WiFi.isConnected()) {
		ledmatrix.turnLed(i % 32, i / 32, true);
		++i;
		Serial.print(".");
		if (i >= 8) {
			Serial.printf("\nNo response from access point\n");
			i = 0;
			WiFi.disconnect();
			WiFi.begin(config.wifiNetwork, config.wifiPassword);
			ledmatrix.clearDisplay();
		}
		delay(500);
	}

	Serial.printf("\nConnected to Wi-Fi - %s\n", WiFi.localIP().toString().c_str());
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
	Serial.printf("Displaying time - %02d:%02d:%02d\n", hours, minutes, seconds);
}

void Application::displayTemperature()
{
	char temp[4];
	snprintf(temp, sizeof(temp), "%.1f", weather.temperature);
	String temperature = (weather.temperature > 0 ? "+" : "") + String(temp) + "°";
	Serial.printf("Displaying temperature - %s\n", temperature.c_str());

	ledmatrix.clearDisplay();
	ledmatrix.renderStringInCenter(temperature, 5000);
	ledmatrix.clearDisplay();
}

void Application::displayCity()
{
	Serial.printf("Displaying city %s\n", weather.city.c_str());
	ledmatrix.clearDisplay();
	ledmatrix.renderFloatingText(weather.city);
	ledmatrix.clearDisplay();
}

void Application::displayDescription()
{
	Serial.printf("Displaying weather description %s\n", weather.description.c_str());
	ledmatrix.clearDisplay();
	ledmatrix.renderFloatingText(weather.description);
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
