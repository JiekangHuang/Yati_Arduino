#include "mcp3428.h"

#define UARTBAUDRATE         115200
#define NUMOFCHANNEL         4
#define CONTINUOUS_MODE      1
#define ONE_SHOT_MODE        0

uint32_t preTime = millis();
void setup()
{
    Serial.begin(UARTBAUDRATE);

    initMCP3428(0x06);
    while (chkMCP3428() == -1);
    Serial.println("MCP3428 CONNECTION !");
    delay(2000);
}

void loop()
{
    mcp3428Config config;
    config.mode = CONTINUOUS_MODE;
    config.rate = 12;
    config.gain = 1;

    int8_t _error;
    for (uint8_t ch = 0; ch < NUMOFCHANNEL; ch++) {
        int16_t adcData = 0;
        setMCP3428Config(ch, &config);
        do {
            _error = readMCP3428AdcData(&adcData);
        } while (_error == -1);

        Serial.print("ch : ");
        Serial.print(ch + 1);
        Serial.print(" , ADC Value = ");
        Serial.println(adcData);
        delay(200);
    }
    Serial.println();
}