#ifndef Espelho_h
#define Espelho_h
#include "Servo.h"

class Espelho
{
public:
	Espelho(int pin1, int pin2, int addr1, int addr2);
	Espelho(int pin1, int pin2, int addr1, int addr2, uint8_t min, uint8_t max);
	void tiltDownMode();
	void normalMode();
	void up();
	void down();
	void left();
	void right();
	void enableTiltDown(int addr3, int addr4);
	void timeToDisable();
private:
	Servo servoX;
	Servo servoY;
	int pin1;
	int pin2;
	int addr1;
	int addr2;
	int addr3;
	int addr4;
	uint8_t minPos = 0;
	uint8_t maxPos = 180;
	uint8_t normalPosX;
  	uint8_t normalPosY;
  	uint8_t tiltDwnPosX;
  	uint8_t tiltDwnPosY;
	bool hasTiltDownPos = false;
	bool editNormalPos = false;
	bool editTiltDownPos = false;
	bool inTiltDownMode = false;
	bool isEnabled = false;
	bool tiltDownIsEnabled = false;
	long lastChange;
	void enable();
	void disable();
	void savePosition();
	void saveTiltDownPosition();
	void goToNormalPosition();
	void goToTiltdownPosition();
	void loadPosition();
	void loadTiltDownPosition();
};
#endif