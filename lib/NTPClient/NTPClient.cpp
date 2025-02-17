#include "NTPClient.h"
#ifdef ARDUINO_ARCH_ESP8266
    #include <ESP8266WiFi.h>
#else
    #include <WiFi.h>
#endif

NTPClient::NTPClient() :
	timeZoneSeconds(),
	ntpServerIp()
{ }

void NTPClient::getTime()
{
	Serial.println("[NTP] Waiting for NTP sync...");

	if (!resolveNtpServerHostname()) {
		Serial.println("[NTP] Error: NTP hostname wasn't resolved");
		return;
	}

	udp.begin(2390);
	sendNTPpacket(); // send an NTP packet to a time server
	// wait to see if a reply is available
	delay(1000);

	int bytesCount;
	int retries = 10;

	while (!(bytesCount = udp.parsePacket())) {
		Serial.println("[NTP] No packet yet...");
		delay(300);

		if (retries == 0) {
			Serial.println("[NTP] No answer from remote NTP server!");
			udp.stop();
			return;
		}

		retries--;
	}
	Serial.println("[NTP] A packet received");
	udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

	//the timestamp starts at byte 40 of the received packet and is four bytes,
	// or two words, long. First, esxtract the two words:

	unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
	unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
	// combine the four bytes (two words) into a long integer
	// this is NTP time (seconds since Jan 1 1900):
	unsigned long secsSince1900 = highWord << 16 | lowWord;

	const unsigned long seventyYears = 2208988800UL;
	unixTime = secsSince1900 - seventyYears;
	Serial.printf("[NTP] Received UNIX time from NTP server - %lu\n", unixTime);
	// print Unix time:
	lastUpdated = millis();
	// print the hour, minute and second:
	Serial.printf("[NTP] The current time is %02d:%02d:%02d GMT+0\n", hours(), minutes(), seconds());
	udp.stop();
}

void NTPClient::setTimeZone(int seconds) {
	Serial.printf("[NTP] Setting timezone GMT %s%d\n", seconds > 0 ? "+" : "-", seconds / 3600);
	timeZoneSeconds = seconds;
}

bool NTPClient::resolveNtpServerHostname() {
	int result = WiFi.hostByName(ntpServerHost.c_str(), ntpServerIp);

	if (result == 1) {
		Serial.printf("[NTP] Resolved IP for NTP server %s - %s\n", ntpServerHost.c_str(), ntpServerIp.toString().c_str());
		return true;
	}

	Serial.printf("[NTP] A problem with resolving NTP server host %s, error code: %d", ntpServerHost.c_str(), result);

	return false;
}

void NTPClient::sendNTPpacket()
{
	Serial.printf("[NTP] Sending NTP packet for IP: %s\n", ntpServerIp.toString().c_str());
	// set all bytes in the buffer to 0
	memset(packetBuffer, 0, NTP_PACKET_SIZE);
	// Initialize values needed to form NTP request
	// (see URL above for details on the packets)
	packetBuffer[0] = 0b11100011;   // LI, Version, Mode
	packetBuffer[1] = 0;     // Stratum, or type of clock
	packetBuffer[2] = 6;     // Polling Interval
	packetBuffer[3] = 0xEC;  // Peer Clock Precision
	// 8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12]  = 49;
	packetBuffer[13]  = 0x4E;
	packetBuffer[14]  = 49;
	packetBuffer[15]  = 52;

	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:
	udp.beginPacket(ntpServerIp, 123); //NTP requests are to port 123
	udp.write(packetBuffer, NTP_PACKET_SIZE);
	udp.endPacket();
}
