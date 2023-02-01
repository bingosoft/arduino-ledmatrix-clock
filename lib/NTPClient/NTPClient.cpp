#include "NTPClient.h"
#include <ESP8266WiFi.h>

NTPClient::NTPClient(String ntpServerIP) :
	ntpServerIP(ntpServerIP) 
{ }

void NTPClient::getTime()
{
	Serial.println("Waiting for NTP sync...");

	ip.fromString(ntpServerIP);

	Serial.printf("Set IP for NTP server %s\n", ip.toString().c_str());

	udp.begin(2390);
	sendNTPpacket(ip); // send an NTP packet to a time server
	// wait to see if a reply is available
	delay(1000);

	int bytesCount;
	int retries = 10;

	while (!(bytesCount = udp.parsePacket())) {
		Serial.println("no packet yet");
		delay(300);

		if (retries == 0) {
			Serial.println("No answer from remote NTP server!");
			udp.stop();
			return;
		}

		retries--;
	}
	Serial.printf("packet received, length=%d\n", bytesCount);
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
	// print Unix time:
	unixTime += currentTimeZone * 60 * 60;
	lastUpdated = millis();
	// print the hour, minute and second:
	Serial.printf("The current time is %02d:%02d:%02d\n", hours(), minutes(), seconds());
	udp.stop();
}

void NTPClient::sendNTPpacket(const IPAddress &address)
{
	Serial.printf("Sending NTP packet for IP: %s\n", address.toString().c_str());
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
	udp.beginPacket(address, 123); //NTP requests are to port 123
	udp.write(packetBuffer, NTP_PACKET_SIZE);
	udp.endPacket();
}
