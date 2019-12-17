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
    config.rate = 16;
    config.gain = 1;

    for (int ch = MCP3428_CH0; ch <= MCP3428_CH3; ch++)
    {
        error = setMCP3428Config(ch, &config);
        while (readMCP3428AdcData(&data) != 0);
        /*error = resetMCP3428();
        error = setMCP3428Latch();
        error = setMCP3428Conversion();*/
        Serial.println(data);
        delay(200);
    }
    delay(1000);
}