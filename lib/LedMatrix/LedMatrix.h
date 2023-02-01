#ifndef LedMatrix_h
#define LedMatrix_h

#include <Arduino.h>

class LedMatrix
{
// digital pins
	int DIN;
	int CLK;
	int CS;
	static const int SECTIONS = 4;
	static const int LINE = 8 * SECTIONS;
	bool state[SECTIONS * 8 * 8] = {}; // each led state

    void clearDisplay(int device);
    void shutdown(int device, bool status);
    void setScanLimit(int device, int limit);
	void render(const byte *rows, int position, int charWidth);
	void update(int device);
	int stringLengthInDots(const String &s) const;
public:
	LedMatrix(int din, int clk, int cs);

    void spiTransfer(int device, byte row, byte data);
    void setIntensity(int device, int intensity);
    void setIntensity(int intensity);
    void clearDisplay();

	int renderChar(int c, int position);
	void renderStringInCenter(const String &s, int duration);
	void renderString(const String &s, int position, int space = 1);
	void renderFloatingText(const String &s, int duration, int updateDelay = 50, int speedDelay = 1000);
	void turnLed(int position, int row, bool on);
};

#endif
