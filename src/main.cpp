#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Application.h"

 // Application digital pins MUST be initized in SETUP func
Application *a;

void setup()
{
	Serial.begin(9600);
	delay(3000);
	a = new Application();
}

void loop()
{
	a->exec();
}
