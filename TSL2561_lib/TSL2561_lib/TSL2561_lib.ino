/*
 Name:		TSL2561_lib.ino
 Created:	2019/12/9 下午 04:41:31
 Author:	s2862
*/
#include "TSL2561.h"

TSL2561 light;
uint16_t ms;
uint8_t gain = 0;

void setup()
{
	Serial.begin(115200);

	light.Power_on();
	if (light.Set_timing(gain, 0, 2, ms));
	Serial.println("Set time ok !");

	uint8_t id;
	if (light.Get_ID(id))
	{
		Serial.print("ID = 0x");
		Serial.println(id, HEX);
		delay(2000);
	}
}

void loop()
{
	uint16_t data0, data1;
	if (light.Get_Data(data0, data1))
	{
		double lux = light.Get_Lux(data0, data1, gain, ms);
		Serial.print("ch0 = ");
		Serial.print(data0);
		Serial.print(" , ch1 = ");
		Serial.print(data1);
		Serial.print(" , Lux = ");
		Serial.println(lux);
	}
	delay(ms);
}