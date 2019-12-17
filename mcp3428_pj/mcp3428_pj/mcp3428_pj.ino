#include "mcp3428.h"

#define UARTBAUDRATE         115200
#define CONVERSION_TIME      60

uint32_t pre, cur;

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
    int32_t adc_data = 0;
    int8_t error;
    mcp3428_reg config;
    config.mode = MCP3428_CONTINUOUS_MODE;
    config.rate = MCP3428_RATE_16;
    config.gain = 1;

    for (uint8_t ch = MCP3428_CH0; ch <= MCP3428_CH3; ch++) {
        setMCP3428Config(ch, &config);
        delay(CONVERSION_TIME);
        readMCP3428AdcData(&adc_data);

        Serial.print("ch : ");
        Serial.print(ch + 1);
        Serial.print(" , ADC Value = ");
        Serial.println(adc_data);
    }
    Serial.println();
    delay(1000);
}