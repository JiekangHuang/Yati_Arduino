/*
 Name:		Task_TEST.ino
 Created:	2019/12/3 下午 04:41:49
 Author:	s2862
*/
#include "DHT.h"

DHT dht11(6, DHT11);
long pre_time, cur_time;
void setup()
{
	Serial.begin(115200);
	pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
	Serial.print("Humidity(%RH): ");
	pre_time = millis();
	uint8_t temp = dht11.readTemperature();
	cur_time = millis();

	Serial.print("delay time = ");
	Serial.print(cur_time - pre_time);
	Serial.println(" ms");
}