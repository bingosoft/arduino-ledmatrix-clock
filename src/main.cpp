#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Application.h"

 // Application digital pins MUST be initized in SETUP func
Application *a;

void setup()
{
	Serial.begin(115200);
	delay(3000);
	a = new Application(Config());
}

void loop()
{
	a->exec();
}
