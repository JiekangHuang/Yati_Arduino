#include "mcp3428.h"

#define UARTBAUDRATE         115200

void setup()
{
    Serial.begin(UARTBAUDRATE);

    initMCP3428(0x06);
    while (chkMCP3428Exist() == ERROR_KEY);
    Serial.println("MCP3428 CONNECTION !");
    delay(2000);
}

void loop()
{
    int8_t error;
    mcp3428_reg config;
    config.mode = MCP3428_CONTINUOUS_MODE;
    config.rate = 12;
    config.gain = 1;

    for (uint8_t ch = MCP3428_CH0; ch <= MCP3428_CH3; ch++) {
        int32_t adc_data = 0;
        error = setMCP3428Config(ch, &config);
        delay(15);
        error = readMCP3428AdcData(&adc_data);

        Serial.print("ch : ");
        Serial.print(ch + 1);
        Serial.print(" , ADC Value = ");
        Serial.println(adc_data);
        delay(200);
    }
    Serial.println();
}