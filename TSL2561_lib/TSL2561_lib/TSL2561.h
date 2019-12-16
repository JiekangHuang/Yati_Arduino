#pragma once
#include <Arduino.h>
#include <Wire.h>

class TSL2561
{
private:
	uint8_t _error;
	bool gain;
	uint8_t time;
public:
	TSL2561();
	~TSL2561();

	void Power_on();
	void Power_off();
	bool Set_timing(bool gain, bool manual, uint8_t time, uint16_t &ms);
	bool Set_interrupt(uint8_t intr, uint8_t persist);

	bool Get_ID(uint8_t &id);
	bool Get_Data(uint16_t &data0, uint16_t &data1);
	double Get_Lux(uint16_t data0, uint16_t data1, bool gain, uint16_t time);
	//-----------------------------------
	bool WriteByte(uint8_t addr, uint8_t value);
	bool ReadByte(uint8_t addr, uint8_t &value);
	bool WriteInt(uint8_t addr, uint16_t value);
	bool ReadInt(uint8_t addr, uint16_t &value);
};

#define TSL2561_ADDR 0x39
#define POWER_ON 0x03
#define POWER_OFF 0x00
#define COMMAND 0X80
#define CONTROL 0x00
#define TIMING 0X01
#define INTERRUPT 0X06
#define ID 0X0A
#define DATA0 0X0C
#define DATA1 0X0E