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
	static const int DOTS_PER_CHAR = 5;
	static const int LINE = 8 * SECTIONS;
	bool state[SECTIONS * 8 * 8] = {}; // each led state

    void clearDisplay(int device);
    void shutdown(int device, bool status);
    void setScanLimit(int device, int limit);
	void render(const byte *rows, int position);
	void update(int device);
	int utf8len(const String &s) const;
public:
	LedMatrix(int din, int clk, int cs);

    void spiTransfer(int device, byte row, byte data);
    void setIntensity(int device, int intensity);
    void setIntensity(int intensity);
    void clearDisplay();

	void renderUtf8Char(int c, int position);
	void renderChar(char c, int position);
	void renderString(const String &s, int position, int space = 1);
	void renderFloatingText(const String &s, int duration, int updateDelay);
	void turnLed(int position, int row, bool on);
};

#endif
