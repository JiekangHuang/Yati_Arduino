#include "mcp3428.h"


void setup()
{
}

void loop()
{
    _writeMCP3428Reg(0x00, 0x00);
}
