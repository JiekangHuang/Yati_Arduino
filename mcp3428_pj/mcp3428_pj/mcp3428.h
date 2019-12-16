#pragma once
#include <Arduino.h>

#ifndef MCP3428_h
#define MCP3428_h

#define MCP3428_RESET        0X06
#define MCP3428_LATCH        0X04
#define MCP3428_CONVERSION   0X08
#define BUFSIZE              3

static uint8_t mcp3428_addr;
static uint8_t sps;
static uint8_t mcp3428Data[BUFSIZE];
static int8_t _error;

typedef struct {
    uint8_t mode;
    uint8_t rate;
    uint8_t gain;
}mcp3428Config;

static int _readMCP3428Reg(uint8_t *arr_data);
static int _writeMCP3428Reg(uint8_t value);

void initMCP3428(uint8_t addr);
int chkMCP3428(void);
int setMCP3428Config(uint8_t channel, mcp3428Config *config);
int readMCP3428AdcData(int16_t *data);
int resetMCP3428(void);
int setMCP3428Latch(void);
int setMCP3428Conversion(void);

#endif // !MCP3428_h
