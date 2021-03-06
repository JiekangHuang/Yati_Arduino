﻿/****************************************************************************
/*
Distributed with a free-will license.
Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
MCP3428
This code is designed to work with the MCP3428_I2CADC I2C Mini Module available from ControlEverything.com.
https://www.controleverything.com/content/Analog-Digital-Converters?sku=MCP3428_I2CADC#tabs-0-product_tabset-2
*/
/****************************************************************************/

#include <MCP3428.h>
#include <Wire.h>

MCP3428 MCP(0x06);
uint32_t pre, cur;
void setup()
{
    Serial.begin(115200);
    Serial.println("MCP3428 Analog to Digital Converter");
    Serial.println("Getting ADC Readings from Channel 1...4");
    Serial.println("        ************************************            ");
    Serial.println("        ");
    delay(500);
    pre = millis();
}

void loop()
{
    pre = millis();
    long Raw_ADC[4];

    for (int i = 1; i <= 4; i++)
    {
        // MCP3428 is configured to channel i with 12 bits resolution, continuous mode and gain defined to 1
        // This arrangement of the mentioned paarmeters can be changed as per convenience
        MCP.SetConfiguration(i, 12, 1, 1);

        // Note that the library waits for a complete conversion
        Raw_ADC[i - 1] = MCP.readADC();
        Serial.print(i);
        Serial.print(" : ");
        Serial.println(Raw_ADC[i - 1]);
    }
    cur = millis();
    Serial.println(cur - pre);
}