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
	//ledmatrix.renderFloatingText("абвгдеёжзиклмнопрстуфхцчшщьыъэюя", 50000000, 300);
	connectToWiFi();

	if (!getTime()) {
		return;
	}

	geoIP.getLocation();
	weather.setLocation(geoIP.latitude, geoIP.longitude, geoIP.city);
	weather.update();

	subscribeTimers();
}

void Application::subscribeTimers() {
	timers.schedule(30000, this, &Application::displayWeather, 10000);
	timers.schedule(30000, this, &Application::displayDescription, 20000);
	timers.schedule(180000, this, &Application::displayCity, 30000);
	timers.schedule(5 * 60000, &weather, &Weather::update);
	timers.schedule(30 * 60000, &ntpClient, &NTPClient::getTime);
}

bool Application::getTime() {
	const int maxRetries = 4;
	int retries = 0;
	ntpClient.getTime();

	while (!ntpClient.hasTime() && retries < maxRetries) {
		delay(500);
		retries++;
		Serial.printf("NTP time still not received. Retry again. Retries: %d\n", retries);
		ntpClient.getTime();
		ledmatrix.turnLed(retries, 1, true);
	}

	if (!ntpClient.hasTime()) {
		ledmatrix.renderFloatingText("ntp error", 100000);
	}

	return ntpClient.hasTime();
}

void Application::connectToWiFi()
{
	Serial.printf("Connecting to Wi-Fi network %s, password %s\n", config.wifiNetwork.c_str(), config.wifiPassword.c_str());

	ledmatrix.renderFloatingText("connecting...", 3000, 50, 100);
	ledmatrix.renderFloatingText(config.wifiNetwork, 3000, 50, 100);
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

void Application::displayWeather()
{
	int t = weather.temperature;
	String temperature = (t > 0 ? "+" : "") + String(t) + "°";
	Serial.printf("Displaying temperature - %s\n", temperature.c_str());

	ledmatrix.clearDisplay();
	ledmatrix.renderStringInCenter(temperature, 5000);
	delay(5000);
	ledmatrix.clearDisplay();
}

void Application::displayCity()
{
	Serial.printf("displaying city %s\n", weather.city.c_str());
	ledmatrix.clearDisplay();
	ledmatrix.renderFloatingText(weather.city, 5000, 50);
	ledmatrix.clearDisplay();
}

void Application::displayDescription()
{
	Serial.printf("Displaying weather description %s\n", weather.description().c_str());
	ledmatrix.clearDisplay();
	ledmatrix.renderFloatingText(weather.description(), 5000, 50);
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
