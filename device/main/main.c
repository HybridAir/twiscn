#include <avr/io.h>
#include <util/delay.h>

void init_io(void) {
	DDRC = 0b00010000; // All outputs
}

int main(void)
{
	init_io();

	while (1)
	{
		PORTC = 0xFF;
		_delay_ms(500);

		PORTC = 0x00;
		_delay_ms(500);
	}

	return 0;
}