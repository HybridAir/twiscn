#include <avr/io.h>
#include <util/delay.h>

#include "io.h"

int main(void)
{
    ioInit();

	while (1)
	{
		PORTC = 0xFF;
		_delay_ms(100);

		PORTC = 0x00;
		_delay_ms(100);
	}

	return 0;
}