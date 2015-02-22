CC=arm-none-eabi-gcc
CFLAGS=-g -Os -Wall -Wextra -fno-common -ffunction-sections -fdata-sections -MD -mthumb -mcpu=cortex-m3 -std=gnu99 -c
LDFLAGS=-nostartfiles -T./stm32f10x.ld -Wl,-Map=main.map -Wl,--gc-sections -mthumb -mcpu=cortex-m3
ASM=arm-none-eabi-as
ASMFLAGS=
SOURCES=main.c pcf8833.c system.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main.elf
OBJDUMP=arm-none-eabi-objdump
SIZE=arm-none-eabi-size
READELF=readelf

all: clean $(SOURCES) $(EXECUTABLE) dump
   
$(EXECUTABLE): startup_stm32f10x_md.o $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) startup_stm32f10x_md.o -o $@

startup_stm32f10x_md.o:
	$(ASM) startup_stm32f10x_md.s -o startup_stm32f10x_md.o

.c.o:
	$(CC) $(CFLAGS) $< -o $@
    

	
clean:
	rm -f *.o
	rm -f *.elf
	rm -f *.d
	rm -f *.map
	rm -f *~
	rm -f dump.s
	rm -f main.symbol
	
dump:
	$(OBJDUMP) -D $(EXECUTABLE) > dump.s
	$(SIZE) $(EXECUTABLE)
	$(READELF) -a ./main.elf > main.symbol

