/*
  RF transmitter example for attiny13.
  The code was created by analyzing these sources:
  1. RCSwitch library, https://code.google.com/p/rc-switch/
  2. http://habrahabr.ru/post/224459/
  3. http://avrbasiccode.wikispaces.com/
  
  For data receving just use examples from RCSwitch.
  
  Don't forget to set up properly frequency for attiny13.
  See Arduino\hardware\attiny\boards.txt
  I've used attiny13.build.f_cpu=1200000L to get proper delays
  
*/

#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/io.h>

#define F_CPU 4800000UL // The internal oscillator speed

const short  nPulseLength = 181; //pulse length matching, the overall lentght of a complete pulse was 373 @ 9800000

const short  nHighPulses_0 = (nPulseLength * 1);
const short nLowPulses_0 = (nPulseLength * 3);

const short nHighPulses_1 = (nPulseLength * 3);
const short nLowPulses_1 = (nPulseLength * 1);

const short nLowPulses_sync =  (nPulseLength * 31);

#define PIN_TX		(1<<PB3) // PB3 pin, goes to transmitter data pin
void sleep() {

    GIMSK |= _BV(PCIE);                     // Enable Pin Change Interrupts
    PCMSK |= _BV(PCINT4);                   // Use PB4 as interrupt pin
    ADCSRA &= ~_BV(ADEN);                   // ADC off
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement

    sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
    sei();                                  // Enable interrupts
    sleep_cpu();                            // sleep

    cli();                                  // Disable interrupts
    PCMSK &= ~_BV(PCINT4);                  // Turn off PB4 as interrupt pin
    sleep_disable();                        // Clear SE bit
    ADCSRA |= _BV(ADEN);                    // ADC on

    sei();                                  // Enable interrupts
    } // sleep

ISR(PCINT0_vect) {
    // This is called when the interrupt occurs, but I don't need to do anything in it
    }


void send(char* sCodeWord){

	while (*sCodeWord != '\0') {
  
		PORTB |= PIN_TX; // same as digitalWrite high

		if(*sCodeWord == '0')
		{
			_delay_us(nHighPulses_0);
			PORTB &= ~PIN_TX; // same as digitalWrite low
			_delay_us(nLowPulses_0);
		}else
		{
			_delay_us(nHighPulses_1);
			PORTB &= ~PIN_TX;
			_delay_us(nLowPulses_1);
		} 

		++sCodeWord;
	}

	PORTB |= PIN_TX;
	_delay_us(nHighPulses_0);

	PORTB &= ~PIN_TX;
	_delay_us(nLowPulses_sync);
}


void setup() {         
	DDRB |= PIN_TX; // Set output direction on PIN_TX
	PORTB |= (1 << PB4) ; // Set internal pullup high
}

int main(void){  
	for (int8_t i = 0; i<33; ++i) // pulse repeat, the amount of times the pulse is repeated
	{
	send("001000010101001110101010"); //0's are short, 1's are long
	sleep();
	}
	
		

	}






