/*
 Name:		test.ino
 Created:	2019/12/17 下午 02:29:09
 Author:	s2862
*/

#include "mcp3428.h"

void setup()
{
    Serial.begin(115200);
    initMCP3428(0x06);
}

void loop()
{
    int8_t error;
    int32_t data;
    mcp3428_reg config;
    config.mode = 1;
    config.rate = 12;
    config.gain = 1;

    error = setMCP3428Config(0, &config);
    delay(20);
    error = readMCP3428AdcData(&data);
    error = resetMCP3428();
    error = setMCP3428Latch();
    error = setMCP3428Conversion();
    Serial.println(error);
}