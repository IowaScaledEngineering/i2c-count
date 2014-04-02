/*************************************************************************
Title:    I2C Counter
Authors:  Michael Petersen <railfan@drgw.net>
File:     i2c-count.c
License:  GNU General Public License v3

LICENSE:
    Copyright (C) 2013 Michael Petersen

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

*************************************************************************/

#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <string.h>

#include "avr-i2c-cmdslave.h"

// 7-bit I2C Address
#define I2C_ADDRESS 0x20

// Project specific I2C configuration
#include "i2c.h"

extern volatile uint8_t i2c_busy;

void processCommands(void)
{
	uint8_t i;
	CmdBuffer cmd;
	while(i2cCmdQueueDepth())
	{
		i2cCmdQueuePop(&cmd);

		// Process commands
		switch(cmd.code)
		{
			case 0x03:	// CLEAR_COUNT
				// Clear any faults
				while(i2c_busy);  // Wait for any pending I2C read transactions to keep them atomic
				ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
				{
					// Write atomically to the register
					I2C_STATUS_CML[0] = 0;
				}
				// Do paged clearing here
				for(i=(0xFF==cmd.page?0:cmd.page); i<(0xFF==cmd.page?I2C_NUMPAGES:cmd.page+1); i++)
				{
					while(i2c_busy);  // Wait for any pending I2C read transactions to keep them atomic
					ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
					{
						// Write atomically to the register
						// FIXME: do actual clearing of count and register
					}
				}
				break;
			default:
				// Do nothing
				break;
		}
	}
}


void init(void)
{
	cli();  // Unset interrupts while configuring
	
#if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) 
	// Set Linduino DC590 connector mux to I2C
	DDRB |= _BV(PB0);
	PORTB |= _BV(PB0);
#endif

	// Generate I2C command lookup table
	uint8_t i = 0;
	int i2cNumCommands = sizeof(i2c_registerMap) / sizeof(i2cCommand);
	
	do {
		uint8_t j;
		i2c_registerIndex[i] = I2C_UNSUPPORTED;  // default
		for(j=0; j<i2cNumCommands; j++)
		{
			if(i == i2c_registerMap[j].cmdCode)
			{
				i2c_registerIndex[i] = j;
			}
		}
		i++;
	} while (i != 0);

	// Set defaults
	I2C_REV_ID[0] = 0x0C00;

	for(i=0; i<I2C_NUMPAGES; i++)
	{
		// FIXME: reset counter registers
	}

	// Initialize I2C last - after all registers are configured
	i2c_slave_init(I2C_ADDRESS, 0);
	
	sei();
}


int main(void)
{
	init();

	while(1)
	{
		processCommands();
	}
}

