#ifndef LedMatrix_h
#define LedMatrix_h

#include <Arduino.h>

class LedMatrix
{
// digital pins
	int DIN;
	int CLK;
	int CS;
	int devices;
	static const int DOTS_PER_CHAR = 5;

    void shutdown(int device, bool status);
    void setScanLimit(int device, int limit);
	void render(const byte *rows, int position);
public:
	LedMatrix(int din, int clk, int cs, int devices = 1);

    void spiTransfer(int device, byte row, byte data);
    void setIntensity(int device, int intensity);
    void setIntensity(int intensity);
    void clearDisplay(int device);

	void renderChar(char c, int position);
};

#endif
