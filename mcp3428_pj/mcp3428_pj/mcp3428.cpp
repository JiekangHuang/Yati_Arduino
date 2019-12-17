#include "mcp3428.h"
#include <Wire.h>
#include <math.h>

static uint8_t   mcp3428_addr;
static uint8_t   sps;
static uint8_t   mcp3428_data[BUFSIZE];
static int8_t    error;

/*
function  : Read the conversion data into arr_data.
parameter : Array of length 3
Return    :
    return = 0 , Output register has been updated with the latest conversion result.
    return = 2 , Output register has not been updated.
*/
static int _readMCP3428Reg(uint8_t * arr_data)
{
    Wire.requestFrom(mcp3428_addr, BUFSIZE);

    uint8_t i = 0;
    uint8_t rdy;
    while (Wire.available()) {
        arr_data[i++] = Wire.read();
        rdy = arr_data[BUFSIZE - 1] >> 7;
    }

    if (rdy == ERROR_NONE) {
        return ERROR_NONE;
    }
    return ERROR_NRDY;
}

/*
function  : Write commamd to MCP3428 Register.
parameter : commamd
Return    :
    return = 0 , Write success.
    return = 1 , Write failed.
*/
static int _writeMCP3428Reg(uint8_t value)
{
    Wire.beginTransmission(mcp3428_addr);
    Wire.write(value);
    error = Wire.endTransmission();
    if (error == ERROR_NONE) {
        return ERROR_NONE;
    }
    return ERROR_KEY;
}

/*
function  : MCP3428 address initialization.
parameter : MCP3428 address(A2,A1,A0)
*/
void initMCP3428(uint8_t addr)
{
    Wire.begin();
    mcp3428_addr = (MCP3428_DEVICE_CODE << 3) | addr;
}

/*
function  : Check if MCP3428 is connected.
parameter : void
Return    :
    return = 0 , connect success.
    return = 1 , Write failed.
*/
int chkMCP3428Exist()
{
    Wire.beginTransmission(mcp3428_addr | 0x01);
    error = Wire.endTransmission();
    if (error == ERROR_NONE) {
        return ERROR_NONE;
    }
    return ERROR_KEY;
}

/*
function  : Setting MCP3428 Configuration Register.
parameter : Channel Selection¡BConversion Mode¡BSample Rate Selection¡BPGA Gain Selection
Return    :
    return = 0 , setting success.
    return = 1 , setting failed.
    return = 3 , input channel error.
    return = 4 , input Conversion mode error.
    return = 5 , input Sample Rate error.
    return = 6 , input PGA Gain error.
*/
int setMCP3428Config(uint8_t channel, mcp3428_reg *config)
{
    sps = config->rate;
    uint8_t mode = config->mode;
    uint8_t rate = config->rate;
    uint8_t gain = config->gain;

    if (channel < MCP3428_CH0 || channel > MCP3428_CH3) {
        return ERROR_CH;
    }
    if (mode != MCP3428_ONE_SHOT_MODE && mode != MCP3428_CONTINUOUS_MODE) {
        return ERROR_MODE;
    }
    if (rate != MCP3428_RATE_12 && rate != MCP3428_RATE_14 && rate != MCP3428_RATE_16) {
        return ERROR_RATE;
    }
    if (gain < MCP3428_GAIN_MIN || gain > MCP3428_GAIN_MAX) {
        return ERROR_GAIN;
    }

    /*Configuration Register
        MSB                            LSB
        RDY | channel | mode | rate | gain
         1       2       1      2      2

         RDY:
         0 = No effect.
         1 = Initiate a new conversion

         channel:
         00 = ch1
         01 = ch2
         10 = ch3
         11 = ch4

         mode:
         0 = One-Shot Conversion Mode
         1 = Continuous Conversion Mode

         Sample Rate:
         00 = 240 SPS(12 bits)(Default)
         01 =  60 SPS(14 bits)
         10 =  15 SPS(16 bits)

         gain:
         00 = x1(Default)
         01 = x2
         10 = x4
         11 = x8
    */
    uint8_t value = 0x00;
    value |= channel;
    value <<= 1;
    value |= mode;
    value <<= 2;
    value |= int(rate - 12) / 2;
    value <<= 2;
    value |= int(log(gain) / log(2));

    return (_writeMCP3428Reg(value | 0x80)); //RDY=1 Initiate a new conversion
}

/*
function  : read adc value.
parameter : variable point
Return    :
    return = 0 , read success.
    return = 1 , read failed(Output register has not been updated).
    return = 2 , sps error.
*/
int readMCP3428AdcData(int32_t *data)
{
    if (_readMCP3428Reg(mcp3428_data) == ERROR_NONE) {
        switch (sps) {
        case 12:
            *data = mcp3428_data[0];
            *data &= B00001111;
            *data <<= 8;
            *data |= mcp3428_data[1];

            if (*data > BIT12_MAX) {
                *data += BIT12_MIN;
            }
            break;
        case 14:
            *data = mcp3428_data[0];
            *data &= B00111111;
            *data <<= 8;
            *data |= mcp3428_data[1];

            if (*data > BIT14_MAX) {
                *data += BIT14_MIN;
            }
            break;
        case 16:
            *data = mcp3428_data[0];
            *data <<= 8;
            *data |= mcp3428_data[1];

            if (*data > BIT16_MAX) {
                *data += BIT16_MIN;
            }
            break;
        default:
            *data = 0;
            return ERROR_RATE;
            break;
        }
        return ERROR_NONE;
    }
    return ERROR_KEY;
}

/*
function  : general call reset.
parameter : void
Return    :
    return = 0 , reset success.
    return = 1 , reset failed.
*/
int resetMCP3428()
{
    if (_writeMCP3428Reg(MCP3428_RESET) == ERROR_NONE) {
        return ERROR_NONE;
    }
    return ERROR_KEY;
}

/*
function  : general call latch.
parameter : void
Return    :
    return = 0 , latch success.
    return = 1 , latch failed.
*/
int setMCP3428Latch()
{
    if (_writeMCP3428Reg(MCP3428_LATCH) == ERROR_NONE) {
        return ERROR_NONE;
    }
    return ERROR_KEY;
}

/*
function  : general call conversion.
parameter : void
Return    :
    return = 0 , conversion success.
    return = 1 , conversion failed.
*/
int setMCP3428Conversion()
{
    if (_writeMCP3428Reg(MCP3428_CONVERSION) == ERROR_NONE) {
        return ERROR_NONE;
    }
    return ERROR_KEY;
}