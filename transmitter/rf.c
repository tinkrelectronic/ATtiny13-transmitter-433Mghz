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


#include <avr/io.h>

#define F_CPU 4800000UL // The internal oscillator speed

const short  nPulseLength = 181; //pulse length matching. At this setting and oscillator speed this is 30.8ms

const short  nHighPulses_0 = (nPulseLength * 1);
const short nLowPulses_0 = (nPulseLength * 3);

const short nHighPulses_1 = (nPulseLength * 3);
const short nLowPulses_1 = (nPulseLength * 1);

const short nLowPulses_sync =  (nPulseLength * 31);
uint8_t readButton(void);

#define PIN_TX		(1<<PB3) // PB3 pin, goes to transmitter data pin
#define SWITCH  PB4

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
	ADCSRA &= ~(1<<ADEN); // Disable ADC
	ACSR = (1<<ACD); //Disable the analog comparator
	DIDR0 = 0x3F; //Disable digital input buffers on all ADC0-ADC5 pins.
}

int main(void) 
{  
		PORTB |= (1 << SWITCH) ; // Set internal pullup high
	while(1) {

		if(readButton() == 1)
		{
		for (int8_t i = 0; i<33; ++i) // pulse repeat, the amount of times the pulse is repeated
		{
		send("001000010101001110101010"); //0's are short, 1's are long
		}
		}
		if(readButton() == 0)
		{
		//do nothing
		}
		
	}
	}






uint8_t readButton(void){
 if((PINB & (1<<SWITCH)) == 0){        //If the button was pressed
 _delay_ms(100); }        //Debounce the read value
 if((PINB & (1<<SWITCH)) == 0){        //Verify that the value is the same that what was read
 return 1; }            //If it is still 0 its because we had a button press
 else{                    //If the value is different the press is invalid
 return 0; }
 
}
