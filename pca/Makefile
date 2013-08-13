TARGET=libpca.a
SOURCES= \
		serial.c \
		common.c \
		slip.c \
		tdelay_common.c \
		tdelay.c \
		beeper.c

DEPS=
COBJ=$(SOURCES:.c=.o)

CC=avr-gcc
AR=avr-ar
STRIP=avr-strip
MCU=atmega328p
CFLAGS=-I. -Iinclude/ -Wall -Os -DF_CPU=16000000UL -std=c99
LDFLAGS=

all: $(TARGET)

%.o: src/%.c $(DEPS)
	@echo -e "\tCC" $<
	@$(CC) -mmcu=$(MCU) -c -o $@ $< $(CFLAGS)

$(TARGET): $(COBJ)
	@echo -e "\tARCHIVING CC" $(COBJ)
	$(STRIP) -g --strip-unneeded $(COBJ)
	$(AR) rcsv $(TARGET) $(COBJ)

clean:
	@echo ========== cleanup ========== 
	rm -rf *.o $(TARGET)	
