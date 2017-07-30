#ifndef NTPCLIENT_H
#define NTPCLIENT_H

#include <WiFiUdp.h>
#include <Arduino.h>

class NTPClient
{
	const char* ntpServerName = "timeserver.ru";
	static const int NTP_PACKET_SIZE = 48;
	const int currentTimeZone = 3; // UTC+3

	WiFiUDP udp;
	IPAddress timeServerIP;
	byte packetBuffer[NTP_PACKET_SIZE];

	unsigned long unixTime = 0;
	unsigned long lastUpdated = 0;
	void sendNTPpacket(const IPAddress &address);

public:
	void update();
	unsigned long secondsSinceEpoch() const { return unixTime + (millis() - lastUpdated) / 1000; }
	int seconds() const { return secondsSinceEpoch() % 60; }
	int minutes() const { return (secondsSinceEpoch() / 60) % 60; }
	int hours() const { return (secondsSinceEpoch() / 60 / 60) % 24; }
	bool hasTime() const { return unixTime > 0; }
};


#endif
