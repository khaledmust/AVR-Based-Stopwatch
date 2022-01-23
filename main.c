#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* Setting the CPU Frequency to 1MHz. */
#define F_CPU 1000000

unsigned char seconds	= 0;
unsigned char minutes	= 0;
unsigned char hours	= 0;
unsigned char pauseFlag = 0;

/* Time count. */
ISR(TIMER1_COMPA_vect)
{
	hours = hours + (minutes / 59);
	minutes = (minutes + (seconds / 59)) % 60;
	seconds = (seconds + 1) % 60;
}

/* Clear the time parameters. */
ISR(INT0_vect)
{
	seconds = 0;
	minutes = 0;
	hours 	= 0;
}

/* Pause the timer. */
ISR(INT1_vect)
{
	/* Disable the clock in order to pause the timer count. */
	TCCR1B = 0;
}

/* Resume the timer. */
ISR(INT2_vect)
{
	/* Enable the clock to resume the timer count. */
	TCCR1B = (1 << WGM12) | (1 << CS10) | (1 << CS11);
}

/* Timer1 configuration. */
void TIMER1_CTC(void)
{
	/* Initial value of timer1. */
	TCNT1 = 0;

	/* Set the compare value to 15624 Clocks:
	 * - Set the prescaler to 64 leads to the T_Timer = 64uS.
	 * - Thus the numbers of clocks required to achieve a delay of 1Sec is 1S/64u - 1 = 15624.
	 * - The subtraction of 1 is done to compensate the extra one clock cycle to rise the Compare flag. */
	OCR1A = 0x3D08;

	/* Enable Timer1 Compare A Interrupt. */
	TIMSK |= (1 << OCIE1A);

	/* Configure of timer1 control registers:
	 * - Set the mode to CTC.
	 * - Set the prescaler to 64.
	 */
	TCCR1B = (1 << WGM12) | (1 << CS10) | (1 << CS11);
}

void display_time(void)
{
	for (int i = 0; i < 6; ++i)
	{
		PORTA = (0x01 << i);
		switch (i)
		{
		case 0:
			PORTC = seconds % 10;
			break;
		case 1:
			PORTC = seconds / 10;
			break;
		case 2:
			PORTC  = minutes % 10;
			break;
		case 3:
			PORTC = minutes / 10;
			break;
		case 4:
			PORTC = hours % 10;
			break;
		case 5:
			PORTC = hours / 10;
			break;
		default:
			break;
		}
		/* Add a delay of 3mS (arbitrary value) for persistence effect of the 7-segments. */
		_delay_ms(3);
	}
}

/* Configure of Interrupt 0. */
void INT0_init(void)
{
	/* Set the direction of PD2 as input. */
	DDRD &= ~(1 << PD2);

	/* Activate the pull-up resistor. */
	PORTD |= (1 << PD2);

	/* Configure the interrupt to trigger with the falling edge. */
	MCUCR |= (1 << ISC01);

	/* Activate the module interrupt. */
	GICR |= (1 << INT0);
}

/* Configure of Interrupt 1. */
void INT1_init(void)
{
	/* Set the direction of PD3 as input. */
	DDRD &= ~(1 << PD3);

	/* Configure the interrupt to trigger with the rising edge. */
	MCUCR |= (1 << ISC11) | (1 << ISC10);

	/* Activate the module interrupt. */
	GICR |= (1 << INT1);
}

/* Configure of Interrupt 2. */
void INT2_init(void)
{
	/* Set the direction of PB2 as input. */
	DDRB &= ~(1 << PB2);

	/* Activate the internal pull-up resistor. */
	PORTB |= (1 << PB2);

	/* Configure of interrupt to trigger at falling edge. */
	MCUCSR |= (1 << ISC2);

	/* Activate the module interrupt. */
	GICR |= (1 << INT2);
}

int main(void)
{
	/* Set pins PA0, PA1, PA2, PA3, PA4, PA5 as output. */
	DDRA |= 0x60;

	/* Set port C as output. */
	DDRC = 0xFF;
	PORTC = 0;

	/* Enable global interrupts. */
	SREG |= (1 << 7);

	/* Initialize external interrupts. */
	INT0_init();
	INT1_init();
	INT2_init();

	/* Initialize and start the timer. */
	TIMER1_CTC();

	while (1)
	{
		display_time();
	}
}
