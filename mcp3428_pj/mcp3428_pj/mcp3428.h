#pragma once
#include <Arduino.h>

#ifndef MCP3428_H
#define MCP3428_H

#define MCP3428_DEVICE_CODE  0x0D
#define MCP3428_RESET        0X06
#define MCP3428_LATCH        0X04
#define MCP3428_CONVERSION   0X08
#define BUFSIZE              3
#define DEFAULT_RATE         12

#define BIT12_MAX            0x7FF
#define BIT14_MAX            0x1FFF
#define BIT16_MAX            INT16_MAX
#define BIT12_MIN            -0x1000
#define BIT14_MIN            -0x4000
#define BIT16_MIN            -(UINT16_MAX+1)

#define MCP3428_CH0              0
#define MCP3428_CH3              3
#define MCP3428_CONTINUOUS_MODE  1
#define MCP3428_ONE_SHOT_MODE    0
#define MCP3428_RATE_12          12
#define MCP3428_RATE_14          14
#define MCP3428_RATE_16          16
#define MCP3428_GAIN_MIN         1
#define MCP3428_GAIN_MAX         4

#define DEGUG

typedef enum {
    ERROR_NONE = 0,
    ERROR_KEY,
    ERROR_NRDY,
    ERROR_CH,
    ERROR_MODE,
    ERROR_RATE,
    ERROR_GAIN
} ERR_CODE;

typedef struct {
    uint8_t mode;
    uint8_t rate;
    uint8_t gain;
}mcp3428_reg;

void initMCP3428(uint8_t addr);
int chkMCP3428Exist(void);
int setMCP3428Config(uint8_t channel, mcp3428_reg *config);
int readMCP3428AdcData(int32_t *data);
int resetMCP3428(void);
int setMCP3428Latch(void);
int setMCP3428Conversion(void);

#endif // !MCP3428_H
