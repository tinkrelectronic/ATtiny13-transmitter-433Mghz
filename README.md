# ATtiny13-transmitter-433Mghz
Simplest way to transmit digital data over radio frequency

Hardware requirements:

1. attiny13 

2. 433MHz/315MHz AM transmitter

3. 17.2cm wire as antenna, gives at least 10m of distance, though you need to send same code several times.

To compile C++ file

1.) avr-g++ -g -Os -mmcu=attiny13 -c rf.c

2.) avr-g++ -g -mmcu=attiny13 -o rf.elf rf.o

3.) avr-objcopy -j .text -j .data -O ihex rf.elf rf.hex

note: ATTINY15A set fuses to L=0X69 H=0xFF

Pin 2 TX
Pin 3 Switch input (switch pulls pin to ground)

For receiver see RCSwitch examples
https://github.com/sui77/rc-switch
