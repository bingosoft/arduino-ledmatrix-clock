#include <Arduino.h>
#include "Application.h"

 // Application digital pins MUST be initized in SETUP func
Application *a;

void setup()
{
	Serial.begin(115200);
	delay(1000);
	a = new Application(App::Config::current);
}

void loop()
{
	a->exec();
}
