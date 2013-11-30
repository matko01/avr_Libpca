TARGET=libpca.a
SOURCES= \
		adc.c \
		serial.c \
		soft_onewire.c \
		common.c \
		slip.c \
		tdelay.c \
		beeper.c \
		tpwm.c \
		twi.c \
		twi_poll.c \
		twi_common.c \
		timer_common.c \
		tdelay_common.c \
		dev_hd44780.c \
		dev_ds18b20.c

DEPS=
COBJ=$(SOURCES:.c=.o)

CC=avr-gcc
AS=avr-as
AR=avr-ar
STRIP=avr-strip
MCU=atmega328p
CFLAGS=-I. -Iinclude/ -Wall -Os -DF_CPU=16000000UL -std=gnu99 \
	   -ffunction-sections \
	   -fdata-sections \
	   -fpack-struct \
	   -fno-tree-scev-cprop \
	   -funsigned-bitfields \
	   -fno-inline-small-functions
LDFLAGS=

all: $(TARGET)

%.o: src/%.c $(DEPS)
	@echo -e "\tCC" $<
	@$(CC) -mmcu=$(MCU) -c -o $@ $< $(CFLAGS)

$(TARGET): $(COBJ)
	@echo -e "\tARCHIVING CC" $(COBJ)
	$(STRIP) -g $(COBJ)
	$(AR) rcsv $(TARGET) $(COBJ)

clean:
	@echo ========== cleanup ========== 
	rm -rf *.s *.o $(TARGET)	
