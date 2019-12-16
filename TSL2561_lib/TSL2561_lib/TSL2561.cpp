#include "TSL2561.h"

TSL2561::TSL2561()
{
	Wire.begin();
}

TSL2561::~TSL2561()
{
}

void TSL2561::Power_on()
{
	this->WriteByte(CONTROL, POWER_ON);
}

void TSL2561::Power_off()
{
	this->WriteByte(CONTROL, POWER_OFF);
}

#define ERR_PARA		1
#define ERR_BUSY		2

/*
	return: 0 - successful
			-1 - error
			-2 -
*/
bool TSL2561::Set_timing(bool gain, bool manual, uint8_t time, uint16_t &ms)
{
	/* check parameters */
	//return -ERR_PARA;

	/* process here */
	switch (time)
	{
	case 0:
		ms = 14;
		break;
	case 1:
		ms = 101;
		break;
	case 2:
		ms = 402;
		break;
	default:
		ms = 402;
		break;
	}

	uint8_t value = 0x00;
	gain ? value |= 0x10 : value &= ~0x10;
	manual ? value |= 0x08 : value &= ~0x08;
	value |= time;

	return (WriteByte(TIMING, value));
}

bool TSL2561::Set_interrupt(uint8_t intr, uint8_t persist)
{
	/* check parameters */
	//return -ERR_PARA;

	/* process here */
	uint8_t value = 0x00;

	intr <<= 4;
	value |= intr;
	value |= persist;

	return (WriteByte(INTERRUPT, value));
}

bool TSL2561::Get_ID(uint8_t & id)
{
	return (ReadByte(ID, id));
}

bool TSL2561::Get_Data(uint16_t & data0, uint16_t & data1)
{
	return (ReadInt(DATA1, data1) && ReadInt(DATA0, data0));
}

double TSL2561::Get_Lux(uint16_t data0, uint16_t data1, bool gain, uint16_t time)
{
	/* check parameters */
	//return -ERR_PARA;

	/* process here */

	if (!gain)
	{
		data0 <<= 4;
		data1 <<= 4;
	}

	double ch0 = (double)data0, ch1 = (double)data1;
	ch0 *= (402.0 / time);
	ch1 *= (402.0 / time);

	//Illumination calculation
	double ratio = ch1 / ch0, lux = 0.0;
	if (ratio <= 0.5)
	{
		lux = (0.0304*ch0) - (0.062*ch0 * pow(ratio, 1.4));
		return (lux);
	}
	if (ratio <= 0.61)
	{
		lux = (0.0224*ch0) - (0.031*ch1);
		return (lux);
	}
	if (ratio <= 0.8)
	{
		lux = (0.0128*ch0) - (0.0153*ch1);
		return (lux);
	}
	if (ratio <= 1.3)
	{
		lux = (0.00146*ch0) - (0.00112*ch1);
		return (lux);
	}
	return (0.0);
}

bool TSL2561::WriteByte(uint8_t addr, uint8_t value)
{
	Wire.beginTransmission(TSL2561_ADDR);
	Wire.write(addr | COMMAND);
	Wire.write(value);
	this->_error = Wire.endTransmission();

	return (this->_error == 0);
}

bool TSL2561::ReadByte(uint8_t addr, uint8_t & value)
{
	Wire.beginTransmission(TSL2561_ADDR);
	Wire.write(addr | COMMAND);
	this->_error = Wire.endTransmission();

	if (!this->_error)
	{
		Wire.requestFrom(TSL2561_ADDR, 1);
		if (Wire.available())
		{
			value = Wire.read();
			return (true);
		}
	}
	return false;
}

bool TSL2561::WriteInt(uint8_t addr, uint16_t value)
{
	Wire.beginTransmission(TSL2561_ADDR);
	Wire.write(addr | COMMAND);
	Wire.write(value);
	this->_error = Wire.endTransmission();

	return (this->_error == 0);
}

bool TSL2561::ReadInt(uint8_t addr, uint16_t & value)
{
	uint16_t low, high;

	Wire.beginTransmission(TSL2561_ADDR);
	Wire.write(addr | COMMAND);
	this->_error = Wire.endTransmission();

	if (!this->_error)
	{
		Wire.requestFrom(TSL2561_ADDR, 2);
		if (Wire.available() == 2)
		{
			low = Wire.read();
			high = Wire.read();

			value = word(high, low);
			return (true);
		}
	}
	return (false);
}