# Top-level Makefile for AVR projects with shared common code

PROGRAM ?= hello_world
MCU = atmega328p
F_CPU = 16000000UL
PORT = /dev/ttyACM0
BAUD = 115200
CC = avr-gcc

CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -std=gnu11 -Wall -Wextra
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

SRC_DIR = $(PROGRAM)
COMMON_DIR = common
SRC = $(SRC_DIR)/main.c $(wildcard $(COMMON_DIR)/*.c)
ELF = $(SRC_DIR)/$(PROGRAM).elf
HEX = $(SRC_DIR)/$(PROGRAM).hex

all: $(HEX)

$(ELF): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: $(HEX)
	$(AVRDUDE) -v -c arduino -p $(MCU) -P $(PORT) -b $(BAUD) -D -U flash:w:$(HEX):i

clean:
	rm -f $(SRC_DIR)/*.elf $(SRC_DIR)/*.hex

.PHONY: all flash clean