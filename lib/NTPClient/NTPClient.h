#ifndef NTPCLIENT_H
#define NTPCLIENT_H

#include <WiFiUdp.h>
#include <Arduino.h>

class NTPClient
{
	static const int NTP_PACKET_SIZE = 48;
	const String ntpServerHost = "time.google.com";
	int timeZoneSeconds;

	IPAddress ntpServerIp;
	WiFiUDP udp;
	byte packetBuffer[NTP_PACKET_SIZE];

	unsigned long unixTime = 0;
	unsigned long lastUpdated = 0;
	void sendNTPpacket();
	bool resolveNtpServerHostname();

public:
	NTPClient();

	void setTimeZone(int seconds);
	void getTime();	
	int secondsSinceLastUpdate() const {
		return (millis() - lastUpdated) / 1000;
	}
	unsigned long secondsSinceEpoch() const {
		return unixTime + timeZoneSeconds + secondsSinceLastUpdate();
	}
	int seconds() const { return secondsSinceEpoch() % 60; }
	int minutes() const { return (secondsSinceEpoch() / 60) % 60; }
	int hours() const { return (secondsSinceEpoch() / 60 / 60) % 24; }
	bool hasTime() const { return lastUpdated > 0; }
};


#endif
