#Blaaaaaaaaa
CC=avr-gcc
hexxer=avr-objcopy

#Bliiiiii
F_CPU=20000000

#Bluuuuuuuu
CFLAGS=-g -Os -mmcu=atmega3290p -c -Wall -std=c99 -DF_CPU=$(F_CPU)
LFLAGS=-g -mmcu=atmega3290p -o 
HFLAGS=-j .text -j .data -O ihex
HFUSES= 0x99
LFUSES= 0x62
EFUSES= 0x00


SOURCES=main.c stuff.c sched.c system.c lcd.c debug.c temp.c
OBJECTS=$(SOURCES:.c=.o)

ELFFILE=elf
HEXFILE=hex

all:	clean compile link toHex

aller: all dude

compile:	
	$(CC) $(CFLAGS) $(SOURCES)	

link: 
	$(CC) $(LFLAGS) $(ELFFILE).elf $(OBJECTS)

toHex:
	$(hexxer) $(HFLAGS) $(ELFFILE).elf $(HEXFILE).hex	 

dude:
	sudo avrdude -p m3290p -c jtagmkII -P usb -e -U flash:w:$(HEXFILE).hex

#fuses:
#	sudo avrdude -p m3290p -c jtagmkII -P usb -U hfuse:w:$(HFUSES):m -u 
#	sudo avrdude -p m3290p -c jtagmkII -P usb -U lfuse:w:$(LFUSES):m -u 
#	sudo avrdude -p m3290p -c jtagmkII -P usb -U efuse:w:$(EFUSES):m -u 

read:
	sudo avrdude -p m3290p -c jtagmkII -P usb -U hfuse:r:-:h -U lfuse:r:-:h efuse:r:-:h -u

clean: 	
	rm -rf *.o *.elf *.hex

