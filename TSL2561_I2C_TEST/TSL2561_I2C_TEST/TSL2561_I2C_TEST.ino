/*
 Name:		TSL2561_I2C_TEST.ino
 Created:	2019/12/5 下午 02:23:30
 Author:	s2862
*/
#include <wire.h>

#define TSL2561_ADDR 0x39
#define TSL2561_ID 0x0A
#define TSL2561_CMD 0x80
#define TSL2561_CONTROL 0x00
#define TSL2561_TIMING 0x01
#define TSL2561_INTERRUPT 0x01
#define TSL2561_DATA0 0x0C
#define TSL2561_DATA1 0x0E
#define TSL2561_POWER_UP 0x03
#define TSL2561_POWER_DOWN 0x00

//Wire.h to I2c
bool ReadByte(uint8_t addr, uint8_t &value);
bool Get_id(uint8_t &id);
bool Get_Data(unsigned int &data0, unsigned int &data1);
bool Set_PowerUP();
bool Set_PowerDOWN();
bool WriteByte(uint8_t addr, uint8_t value);
bool ReadUint(uint8_t addr, unsigned int &value);
bool Set_Timing(uint8_t gain, uint8_t time);
bool manualStart(void);
bool manualStop(void);

//GPIO to I2c
#define SCL GPIO27
#define SDA GPIO28

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
bool i2c_read_ack(void);
void i2c_send_ack(void);
void i2c_send_nack(void);
bool i2c_ack_check(uint8_t data);
uint8_t i2c_readByte(void);
void i2c_writeByte(uint8_t data);

//------------
byte error;

void setup()
{
	Serial.begin(115200);
	Wire.begin();

	unsigned char id;
	if (Get_id(id))
	{
		Serial.print("ID : ");
		Serial.println(id, HEX);
	}
	else
		Serial.println("error !!");
	if (Set_PowerUP())
		Serial.println("power on!!");

	if (Set_Timing(0, 2))
		Serial.println("Set Time is ok !!");
	delay(2000);

	i2c_init();
}

void loop()
{
	unsigned int data0, data1;

	/*if (Get_Data(data0, data1))
	{
		Serial.print("Data0 : ");
		Serial.print(data0);
		Serial.print(" , Data1 : ");
		Serial.println(data1);
	}*/

	uint8_t id;
	i2c_start();
	i2c_writeByte(TSL2561_ADDR);
	i2c_writeByte(TSL2561_ID & 0x80);

	if (!i2c_read_ack())
		Serial.println("read ack !");
	id = i2c_readByte();
	if (id)
		i2c_send_ack();
	i2c_stop();
	Serial.println(id);
}

bool Get_id(uint8_t & id)
{
	if (ReadByte(TSL2561_ID, id))
		return (true);
	return (false);
}

bool Get_Data(unsigned int & data0, unsigned int & data1)
{
	if (ReadUint(TSL2561_DATA0, data0) && ReadUint(TSL2561_DATA1, data1))
		return (true);
	return (false);
}

bool ReadByte(uint8_t addr, uint8_t & value)
{
	Wire.beginTransmission(TSL2561_ADDR);
	Wire.write(addr | TSL2561_CMD);
	error = Wire.endTransmission();

	if (error == 0)
	{
		Wire.requestFrom(TSL2561_ADDR, 1);
		if (Wire.available() == 1)
		{
			value = Wire.read();
			return (true);
		}
	}
	return (false);
}

bool WriteByte(uint8_t addr, uint8_t value)
{
	Wire.beginTransmission(TSL2561_ADDR);
	Wire.write(addr | TSL2561_CMD);
	Wire.write(value);
	error = Wire.endTransmission();

	if (error == 0)
		return (true);
	return (false);
}

bool ReadUint(uint8_t addr, unsigned int & value)
{
	uint8_t high, low;

	Wire.beginTransmission(TSL2561_ADDR);
	Wire.write(addr | TSL2561_CMD);
	error = Wire.endTransmission();

	if (error == 0)
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

bool Set_Timing(uint8_t gain, uint8_t time)
{
	uint8_t value;
	if (gain)
		value |= 0x10;
	else
		value &= ~0x10;

	value |= time;
	if (WriteByte(TSL2561_TIMING, value))
		return (true);
	return (false);
}

bool manualStart(void)
{
	uint8_t time = 0x03;
	if (WriteByte(TSL2561_TIMING, time))
	{
		time |= 0x03;
		if (WriteByte(TSL2561_TIMING, time))
		{
			time |= 0x08;
			if (WriteByte(TSL2561_TIMING, time))
				return (true);
		}
	}
	return (false);
}

bool manualStop(void)
{
	uint8_t time = 0x03;
	if (WriteByte(TSL2561_TIMING, time))
	{
		time &= ~0x08;
		if (WriteByte(TSL2561_TIMING, time))
			return (true);
	}
	return (false);
}

bool Set_PowerUP()
{
	return (WriteByte(TSL2561_CONTROL, TSL2561_POWER_UP));
}

bool Set_PowerDOWN()
{
	return (WriteByte(TSL2561_CONTROL, TSL2561_POWER_DOWN));
}

void i2c_init(void)
{
	pinMode(SCL, INPUT);
	pinMode(SDA, INPUT);
	delayMicroseconds(20);
}

void i2c_start(void)
{
	pinMode(SDA, OUTPUT);
	pinMode(SCL, OUTPUT);

	digitalWrite(SDA, HIGH);
	digitalWrite(SCL, HIGH);
	delayMicroseconds(5);
	digitalWrite(SDA, LOW);
	delayMicroseconds(5);
}

void i2c_stop(void)
{
	pinMode(SDA, OUTPUT);

	if (digitalRead(SCL))
		digitalWrite(SCL, LOW);
	if (digitalRead(SDA))
		digitalWrite(SDA, LOW);

	delayMicroseconds(5);
	digitalWrite(SCL, HIGH);
	delayMicroseconds(5);
	digitalWrite(SDA, HIGH);
	delayMicroseconds(5);

	pinMode(SDA, INPUT);
	pinMode(SCL, INPUT);
}

bool i2c_read_ack(void)
{
	bool ack;
	pinMode(SDA, INPUT);
	digitalWrite(SCL, HIGH);
	ack = digitalRead(SDA);
	delayMicroseconds(5);
	digitalWrite(SCL, LOW);
	delayMicroseconds(5);

	return ack;
}

void i2c_send_ack(void)
{
	pinMode(SDA, OUTPUT);
	digitalWrite(SCL, LOW);
	digitalWrite(SDA, LOW);

	delayMicroseconds(5);
	digitalWrite(SCL, HIGH);
	delayMicroseconds(5);
	digitalWrite(SCL, LOW);
	delayMicroseconds(5);
}

void i2c_send_nack(void)
{
	pinMode(SDA, OUTPUT);
	digitalWrite(SCL, LOW);
	digitalWrite(SDA, HIGH);

	delayMicroseconds(5);
	digitalWrite(SCL, HIGH);
	delayMicroseconds(5);
	digitalWrite(SCL, LOW);
	delayMicroseconds(5);
}

bool i2c_ack_check(uint8_t data)
{
	i2c_start();
	i2c_writeByte(data);
	if (!i2c_read_ack())
	{
		delayMicroseconds(30);
		pinMode(SDA, INPUT);
		pinMode(SCL, INPUT);
		return (false);
	}
	else
	{
		delayMicroseconds(100);
		return (true);
	}
}

uint8_t i2c_readByte(void)
{
	uint8_t data = 0x00;
	delayMicroseconds(5);
	pinMode(SDA, INPUT);
	delayMicroseconds(5);

	for (int i = 0; i < 8; i++)
	{
		data <<= 1;
		digitalWrite(SCL, HIGH);
		if (digitalRead(SDA))
			data |= 0x01;
		else
			data &= ~0x01;
		delayMicroseconds(5);
		digitalWrite(SCL, LOW);
		delayMicroseconds(5);
	}
	return data;
}

void i2c_writeByte(uint8_t data)
{
	pinMode(SDA, OUTPUT);
	pinMode(SCL, OUTPUT);
	for (int i = 0; i < 8; i++)
	{
		digitalWrite(SCL, LOW);
		delayMicroseconds(5);
		if (data & 0x80)
			digitalWrite(SDA, HIGH);
		else
			digitalWrite(SDA, LOW);
		digitalWrite(SCL, HIGH);
		delayMicroseconds(5);
		data <<= 1;
	}
	digitalWrite(SCL, LOW);
	delayMicroseconds(5);
}