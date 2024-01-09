#ifndef LedMatrix_h
#define LedMatrix_h

#include <Arduino.h>
#include <CharMapping.h>

class LedMatrix
{
	enum Operation{
		DecodeMode = 9,
		Intencity = 10,
		ScanLimit = 11,
		ShutDown = 12,
		DisplayTest = 15
	};

	int dataInPin;
	int clkPin;
	int csPin;
	static const int sections = 4;
	static const int lineInDots = 8 * sections;
	bool state[sections * 8 * 8] = {}; // each led state
	CharMapping mapping;

    void clearDisplay(int device);
    void shutdown(int device, bool status);
    void setScanLimit(int device, int limit);
	void render(const byte *rows, int position, int charWidth, bool updateDisplay);
	void update(int device);
    void spiTransfer(int device, Operation operation, byte data);
    void spiTransfer(int device, int row, byte data);
    void setIntensity(int device, int intensity);
	int stringLengthInDots(const String &s) const;

public:
	LedMatrix(int din, int clk, int cs);

    void setIntensity(int intensity);
    void clearDisplay();

	int renderChar(int c, int position, bool updateDisplay = true);
	void renderStringInCenter(const String &s, int duration);
	void renderString(const String &s, int position = 0);
	void renderFloatingText(const String &s, int startDelay = 800, int speedDelay = 40);
	void turnLed(int position, int row, bool on);
	void renderAllSupportedSymbols();
};

#endif
