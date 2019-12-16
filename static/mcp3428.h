

#include <Arduino.h>
#include <Wire.h>

#define MCP3428_ADDR 0X06
#define MCP3428_RESET 0X06
#define MCP3428_LATCH 0X04
#define MCP3428_CONVERSION 0X08

static int _readMCP3428Reg(uint8_t addr, uint8_t *arr_data);
static int _writeMCP3428Reg(uint8_t addr, uint8_t value);

void initMCP3428(uint8_t addr);
int chkMCP3428(uint8_t addr);
int setMCP3428Config(uint8_t Channel, uint8_t mode, uint8_t rate, uint8_t gain);
int readMCP3428AdcData(uint8_t sps, uint16_t *data);
int resetMCP3428(void);
int setMCP3428Latch(void);
int setMCP3428Conversion(void);
