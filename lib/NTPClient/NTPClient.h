#ifndef NTPCLIENT_H
#define NTPCLIENT_H

#include <WiFiUdp.h>
#include <Arduino.h>

class NTPClient
{
	static const int NTP_PACKET_SIZE = 48;
	const int currentTimeZone = 1; // UTC+3
	String ntpServerIP;

	WiFiUDP udp;
	IPAddress ip;
	byte packetBuffer[NTP_PACKET_SIZE];

	unsigned long unixTime = 0;
	unsigned long lastUpdated = 0;
	void sendNTPpacket(const IPAddress &address);

public:
	NTPClient(String ntpServerIP);

	void getTime();
	int secondsSinceLastUpdate() const { return (millis() - lastUpdated) / 1000; }
	unsigned long secondsSinceEpoch() const { return unixTime + secondsSinceLastUpdate(); }
	int seconds() const { return secondsSinceEpoch() % 60; }
	int minutes() const { return (secondsSinceEpoch() / 60) % 60; }
	int hours() const { return (secondsSinceEpoch() / 60 / 60) % 24; }
	bool hasTime() const { return unixTime > 0; }
};


#endif
