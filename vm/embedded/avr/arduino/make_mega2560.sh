#!/bin/sh
# ==============================================================================
# === Configuration ============================================================

BOARD="mega2560"
FLAGS="
-DIMAGE_MODE=roflash
-DBAUD=19200
-DCELL=int16_t
-DIMAGE_SIZE=32000
-DIMAGE_CACHE_SIZE=1300
-DCELL_CACHE_TYPE=uint16_t
-DADDRESSES=128
-DSTACK_DEPTH=128
-DPORTS=15
-DSTRING_BUFFER_SIZE=32
-DSPI_PORT=PORTB
-DSPI_DDR=DDRB
-DSPI_MISO=PB3
-DSPI_MOSI=PB2
-DSPI_SCK=PB1
-DSPI_SS=PB0
-DDISPLAY_PORT=PORTL
-DDISPLAY_DDR=DDRL
-DDISPLAY_SS=PL2
-DDISPLAY_DC=PL1
-DDISPLAY_RST=PL0
-DSDCARD_PORT=SPI_PORT
-DSDCARD_DDR=SPI_DDR
-DSDCARD_SS=SPI_SS
-DDISPLAY=nokia3110
"
CFLAGS="-O2 -Wall -Wextra -std=gnu99"
NATCFLAGS="$FLAGS -DBOARD=native"
AVRCFLAGS="$FLAGS -DBOARD=$BOARD -DAVRCPU -DF_CPU=16000000UL -mmcu=at$BOARD"
IMAGE_BLOCK_SIZE="256"
ROOT_DIRECTORY="../../../.."
LIBRARY_PATH="$ROOT_DIRECTORY/library"
MODULES="
$LIBRARY_PATH/diet.rx
$LIBRARY_PATH/struct.rx
$LIBRARY_PATH/array.rx
$LIBRARY_PATH/hash.rx
$LIBRARY_PATH/stack.rx
$LIBRARY_PATH/bad.rx
$LIBRARY_PATH/bstrings.rx
$LIBRARY_PATH/char.rx
$LIBRARY_PATH/linkedList.rx
$LIBRARY_PATH/combinators.rx
$LIBRARY_PATH/decorator.rx
$LIBRARY_PATH/math.rx
$LIBRARY_PATH/variations.rx
$LIBRARY_PATH/values.rx
$LIBRARY_PATH/locals.rx
library_arduino.rx
library_main.rx
library_nokia3110.rx
library_keypad.rx
library_finish.rx
"
CC="gcc"
AVRCC="avr-gcc"
AVROBJCOPY="avr-objcopy"

# ==============================================================================
. ./build_binaries.sh
