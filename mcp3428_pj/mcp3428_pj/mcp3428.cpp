#include "mcp3428.h"
#include <Wire.h>
#include <math.h>

int _readMCP3428Reg(uint8_t * arr_data)
{
    Wire.requestFrom(mcp3428_addr, BUFSIZE);

    uint8_t i = 0;
    uint8_t rdy;
    while (Wire.available()) {
        arr_data[i++] = Wire.read();
        rdy = arr_data[BUFSIZE - 1] >> 7;
    }
    if (rdy == 0) {
        return 0;
    }
    return -1;
}

int _writeMCP3428Reg(uint8_t value)
{
    Wire.beginTransmission(mcp3428_addr);
    Wire.write(value);
    _error = Wire.endTransmission();
    if (_error == 0) {
        return 0;
    }
    return -1;
}

void initMCP3428(uint8_t addr)
{
    Wire.begin();
    mcp3428_addr = (0x0D << 3) | addr;
    sps = 12;
}

int chkMCP3428()
{
    Wire.beginTransmission(mcp3428_addr | 0x01);
    if (Wire.endTransmission() == 0) {
        return 0;
    }
    return -1;
}

int setMCP3428Config(uint8_t channel, mcp3428Config *config)
{
    uint8_t mode = config->mode;
    uint8_t rate = config->rate;
    uint8_t gain = config->gain;

    if (channel < 0 || channel>3) {
        return 1;
    }
    if (mode != 0 && mode != 1) {
        return 2;
    }
    if (rate != 12 && rate != 14 && rate != 16) {
        return 3;
    }
    if (gain < 0 || gain>3) {
        return 4;
    }

    uint8_t value = 0x00;
    value |= channel;
    value <<= 1;
    value |= mode;
    value <<= 2;
    value |= int(rate - 12) / 2;
    value <<= 2;
    value |= int(log(gain) / log(2));

    return (_writeMCP3428Reg(value | 128));
}

int readMCP3428AdcData(int16_t * data)
{
    _error = _readMCP3428Reg(mcp3428Data);
    if (_error == 0) {
        switch (sps)
        {
        case 12:
            *data = mcp3428Data[0];
            *data &= 0x0F;
            *data <<= 8;
            *data |= mcp3428Data[1];

            if (*data > 2047) {
                *data -= 4096;
            }
            break;
        case 14:
            *data = mcp3428Data[0];
            *data &= 0x3F;
            *data <<= 8;
            *data |= mcp3428Data[1];

            if (*data > 8191) {
                *data -= 16384;
            }
            break;
        case 16:
            *data = mcp3428Data[0];
            *data <<= 8;
            *data |= mcp3428Data[1];

            if (*data > 32767) {
                *data -= 65536;
            }
            break;
        default:
            *data = 0;
            return -1;
            break;
        }
        return 0;
    }
    return _error;
}

int resetMCP3428()
{
    if (_writeMCP3428Reg(MCP3428_RESET) == 0) {
        return 0;
    }
    return -1;
}

int setMCP3428Latch()
{
    if (_writeMCP3428Reg(MCP3428_LATCH) == 0) {
        return 0;
    }
    return -1;
}

int setMCP3428Conversion()
{
    if (_writeMCP3428Reg(MCP3428_CONVERSION) == 0) {
        return 0;
    }
    return -1;
}