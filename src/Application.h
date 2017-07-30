#ifndef APPLICATION_H
#define APPLICATION_H

#include <LedMatrix.h>
#include <Arduino.h>

class Application
{
	// GPIO digital pins
	const int DATA_IN = 13;
	const int CS = 15;
	const int CLK = 14;
	LedMatrix *l;

	bool dotVisible = false;
	int hours = 22;
	int minutes = 59;
	int seconds = 45;

	void updateTime();

public:
	Application();
	void exec();
};

#endif
