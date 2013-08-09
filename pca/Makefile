TARGET=libpca.a
SOURCES= \
		serial.c \
		slip.c

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
	@echo -e "\tARCHIVING CC" $<
	$(AR) rcsv $(TARGET) $(COBJ)
	$(STRIP) --strip-debug --strip-unneeded $(TARGET)

clean:
	@echo ========== cleanup ========== 
	rm -rf *.o $(TARGET)	
