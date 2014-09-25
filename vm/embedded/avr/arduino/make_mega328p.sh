#!/bin/sh
# ==============================================================================
# === Configuration ============================================================

BOARD="mega328p"
FLAGS="
-DIMAGE_MODE=roflash
-DBAUD=19200
-DCELL=int16_t
-DIMAGE_SIZE=32000
-DIMAGE_CACHE_SIZE=254
-DCELL_CACHE_TYPE=uint8_t
-DADDRESSES=75
-DSTACK_DEPTH=100
-DPORTS=15
-DSPI_PORT=PORTB
-DSPI_DDR=DDRB
-DSPI_MISO=PB3
-DSPI_MOSI=PB2
-DSPI_SCK=PB1
-DSPI_SS=PB0
"
#-DSDCARD_PORT=SPI_PORT
#-DSDCARD_DDR=SPI_DDR
#-DSDCARD_SS=SPI_SS
#-DSTRING_BUFFER_SIZE=32

CFLAGS="-Os -Wall -Wextra -std=gnu99"
NATCFLAGS="$FLAGS -DBOARD=native"
AVRCFLAGS="$FLAGS -DBOARD=$BOARD -DAVRCPU -DF_CPU=16000000UL -mmcu=at$BOARD"
AVRCFLAGS="$AVRCFLAGS -mtiny-stack -fno-jump-tables -fconserve-stack"
IMAGE_BLOCK_SIZE="9600"
ROOT_DIRECTORY="../../../.."
LIBRARY_PATH="$ROOT_DIRECTORY/library"
MODULES="
library_arduino.rx
$LIBRARY_PATH/diet.rx
$LIBRARY_PATH/math.rx
library_main.rx
library_finish.rx
"
CC="gcc"
AVRCC="avr-gcc"
AVROBJCOPY="avr-objcopy"

# ==============================================================================
. ./build_binaries.sh
