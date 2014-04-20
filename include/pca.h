#ifndef PCA_H_YDRXPN0M
#define PCA_H_YDRXPN0M

/* Copyright (C) 
 * 2013 - Tomasz Wisniewski
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 */


/**
 * @mainpage Introduction & Library overview

 * The library is at the moment at a very early stage (alpha), but it is functional. The idea behind it is to simplify using Atmega328p peripherals, 
 * by creating a set of simple routines in order to configure and use it quickly. At the moment it has been tested and developed mostly on Arduino UNO rev 3 board, and 
 * for the time being is dedicated to this hardware.The library is written completely in C and is dedicated to speed up everyday C coding for Arduino platform without
 * having a need to use the Arduino Library, which for the cost of simplifying development process (which is good), separates the user from the hardware quite mostly 
 * (which isn't that good). 
 * 
 * @section features Features
 *
 * For the moment the following functionalities are implemented and tested:
 *
 * <ul>
 * <li>Timer Delays (Arduino Library sleep/millis equivalent)</li>
 * <li>Beeper - Tone generation (Arduino Library tone equiv.)</li>
 * <li>Serial port support - Interrupt driven data reception, data reception by polling, data transmission by polling</li>
 * <li>SLIP protocol - serial line data synchronization protocol, to support binary data transfers over serial line</li>
 * </ul>
 *
 * Experimental features (development ongoing)
 *
 * <ul>
 * <li>Timer PWM support</li>
 * <li>ADC API - to be implemented</li>
 * </ul>
 *
 * @section usage Using the documentation
 *
 * For details regarding particular functionality please refer directly to a specific interface file.
 *
 * @section compilation Compiling with your own project
 *
 * It is very simple to use this library with your own project. You can customize the config.h settings, pre-build it and then just simply link your project against
 *  the static library. Or you can include it as a compilation target in your Makefile. Below is an example Makefile for a simple project haing only one file: main.c.
 *  The following directory structure has been assumed:
 *
 * @verbatim
 *  .
 *  | -- project
 *  | ---- Makefile
 *  | ---- main.c
 *  | -- pca
 *  | ---- ...
 *
 * @endverbatim
 *
 * @code
 
  TARGET=project_executable
  SOURCES=main.c 
  
  DEPS=
  COBJ=$(SOURCES:.c=.o)
  PCA_PREFIX=../pca
  
  CC=avr-gcc
  OBJC=avr-objcopy
  MCU=atmega328p
  CFLAGS=-I. -I$(PCA_PREFIX)/include/ -Wall -Os -DF_CPU=16000000UL -std=c99
  LDFLAGS=-lpca -L$(PCA_PREFIX)
  
  all: $(TARGET)
  
  %.o: %.c $(DEPS)
  	@echo -e "\tCC" $<
  	@$(CC) -mmcu=$(MCU) -c -o $@ $< $(CFLAGS)
  
  libpca.a:
  	@echo -e "\tBUILDING PURE C ARDUINO LIB"
  	$(MAKE) -C $(PCA_PREFIX)
  
  $(TARGET): $(COBJ) libpca.a
  	@echo -e "\tLINKING CC" $<
  	@$(CC) -mmcu=$(MCU) -o $(TARGET) $(COBJ) $(LDFLAGS)
  	$(OBJC) -O ihex -R .eeprom $(TARGET) $(TARGET).hex
  
 * @endcode
 *
 */

// ================================================================================

#include <config.h>
#include <common.h>

#include <serial.h>
#include <slip.h>
#include <timer_common.h>
#include <tdelay_common.h>

#include <tdelay.h>
#include <beeper.h>
#include <tpwm.h>
#include <adc.h>
#include <twi.h>
#include <soft_onewire.h>
#include <gpio.h>
#include <spi_hw_poll.h>

#include <dev_ds1307.h>
#include <dev_ds18b20.h>
#include <dev_hd44780.h>
#include <dev_tmp36.h>
#include <dev_d7seg.h>
#include <dev_pcd8544.h>

// ================================================================================

#endif /* PCA_H_YDRXPN0M */

