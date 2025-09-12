MCU = atmega328p
F_CPU = 16000000UL
PORT = /dev/ttyACM0
BAUD = 115200

CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -std=gnu11 -Wall -Wextra

TARGET = main
ELF = $(TARGET).elf
HEX = $(TARGET).hex

all: $(HEX)

SRCS = main.c serial.c analog.c
$(ELF): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS)

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: $(HEX)
	$(AVRDUDE) -v -c arduino -p $(MCU) -P $(PORT) -b $(BAUD) -D -U flash:w:$(HEX):i

clean:
	rm -f $(ELF) $(HEX)
