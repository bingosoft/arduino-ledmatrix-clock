#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Application.h"

 // Application digital pins MUST be initized in SETUP func
Application *a;

void setup()
{
	Serial.begin(115200);
	delay(1000);
	a = new Application(Config::d1Mini);
}

void loop()
{
	a->exec();
}
