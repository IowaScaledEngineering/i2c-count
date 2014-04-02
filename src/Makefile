#*************************************************************************
#Title:    MRB-AVRTEMPLATE Makefile
#Authors:  Michael Petersen <railfan@drgw.net>
#          Nathan Holmes <maverick@drgw.net>
#File:     MRB-AVRTEMPLATE Makefile
#License:  GNU General Public License v3
#
#LICENSE:
#    Copyright (C) 2012 Nathan Holmes and Michael Petersen
#
#    Derived from example WinAVR project Makefile
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 3 of the License, or
#    any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#    
#    You should have received a copy of the GNU General Public License along 
#    with this program. If not, see http://www.gnu.org/licenses/
#    
#*************************************************************************

BASE_NAME = i2c-count

# ATmega328p 16MHz
DEVICE  = atmega328p
F_CPU   = 16000000  # Hz
FUSE_L  = 0xFF
FUSE_H  = 0xDE
FUSE_E  = 0x05

# I2C_CMD_BUFFER_SIZE must be a power of 2 (% in depth function)
DEFINES = -DI2C_NUMPAGES=2 -DI2C_CMD_BUFFER_SIZE=4 -DI2C_ENABLE_PAGE -DI2C_ENABLE_CML
LDFLAGS = 
SRCS = $(BASE_NAME).c i2c/avr-i2c-cmdslave.c
INCS = i2c.h i2c/avr-i2c-cmdslave.h

AVRDUDE = avrdude -c usbtiny -p $(DEVICE) -B1
AVRDUDE_SLOW = avrdude -c usbtiny -p $(DEVICE) -B32
#AVRDUDE = avrdude -c dragon_isp -P usb -p $(DEVICE) -B1
#AVRDUDE_SLOW = avrdude -c dragon_isp -P usb -p $(DEVICE) -B32

OBJS = ${SRCS:.c=.o}
INCLUDES = -I. -I./i2c/
CFLAGS  = $(INCLUDES) -Wall -Os -std=gnu99

COMPILE = avr-gcc $(DEFINES) -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE)

# -E -dM options preprocess only and output results of #define
#COMPILE = avr-gcc -Wall -Os -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(DEVICE) -E -dM

help:
	@echo "make hex ....... build $(BASE_NAME).hex"
	@echo "make flash ..... flash the firmware"
	@echo "make fuse ...... flash the fuses"
	@echo "make program ... flash fuses and firmware"
	@echo "make read ...... read the fuses"
	@echo "make size ...... memory usage"
	@echo "make clean ..... delete objects and hex file"
	@echo "make terminal... open up avrdude terminal"

hex: $(BASE_NAME).hex

program: fuse flash

terminal:
	$(AVRDUDE_SLOW) -t

# rule for programming fuse bits:
fuse:
	@[ "$(FUSE_H)" != "" -a "$(FUSE_L)" != "" -a "$(FUSE_E)" != "" ] || \
		{ echo "*** Invalid Fuse values."; exit 1; }
	$(AVRDUDE_SLOW) -U hfuse:w:$(FUSE_H):m -U lfuse:w:$(FUSE_L):m -U efuse:w:$(FUSE_E):m

read:
	$(AVRDUDE) -v

size:
	avr-size -C --mcu=$(DEVICE) $(BASE_NAME).elf

# rule for uploading firmware:
flash: $(BASE_NAME).hex
	$(AVRDUDE) -U flash:w:$(BASE_NAME).hex:i

# rule for deleting dependent files (those which can be built by Make):
clean:
	rm -f $(BASE_NAME).hex $(BASE_NAME).lst $(BASE_NAME).obj $(BASE_NAME).cof $(BASE_NAME).list $(BASE_NAME).map $(BASE_NAME).eep.hex $(BASE_NAME).elf $(BASE_NAME).s $(OBJS) *.o *.tgz *~

# Generic rule for compiling C files:
.c.o: $(INCS)
	$(COMPILE) -c $< -o $@

# Generic rule for assembling Assembler source files:
.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

# Generic rule for compiling C to assembler, used for debugging only.
.c.s:
	$(COMPILE) -S $< -o $@

# file targets:

$(BASE_NAME).elf: $(OBJS)
	$(COMPILE) -o $(BASE_NAME).elf $(OBJS) $(LDFLAGS)

$(BASE_NAME).hex: $(BASE_NAME).elf
	rm -f $(BASE_NAME).hex $(BASE_NAME).eep.hex
	avr-objcopy -j .text -j .data -O ihex $(BASE_NAME).elf $(BASE_NAME).hex
	avr-size $(BASE_NAME).hex

# debugging targets:

disasm:	$(BASE_NAME).elf
	avr-objdump -d $(BASE_NAME).elf

PWD := $(shell pwd)
