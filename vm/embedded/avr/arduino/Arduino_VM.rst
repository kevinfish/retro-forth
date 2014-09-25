==========================================
Ngaro VM implementation for Arduino boards
==========================================

--------
Abstract
--------
Arduino[1]_ is an open-source electronics prototyping platform based on
flexible, easy-to-use hardware and software. It's intended for artists,
designers, hobbyists, and anyone interested in creating interactive objects or
environments.

This is the Ngaro VM implementation for this platform, which allowes to write
programms in Retro[2]_ langugage, instead of the official Arduino language.

-----------------------------
Configuration and compilation
-----------------------------

To configure and build the VM, you need to create a configuration script. The
easiest way for it, is to copy one from one of the prepared configuration
files:

+------------------+-------------------------------------------------------+
| File name        | Configuration description                             |
+==================+=======================================================+
| make_mega2560.sh | For Arduino MEGA 2560 based boards, with many Retro   |
|                  | libraries and additions.                              |
+------------------+-------------------------------------------------------+
| make_mega328p.sh | For Arduino Nano 2.x/3.x, because of less RAM it is a |
|                  | reduced configuration.                                |
+------------------+-------------------------------------------------------+

This script needs to set following configuration variables:

+------------------+-------------------------------------------------------+
| Variable name    | Description                                           |
+==================+=======================================================+
| BOARD            | The board type:                                       |
|                  | * "mega2560" for the MEGA 2560 board                  |
|                  | * "mega328p" for the Nano board                       |
+------------------+-------------------------------------------------------+
| FLAGS            | Variable which should be included in $NATCFLAGS and   |
|                  | $AVRCFLAGS. It configures the VM and is main          |
|                  | configuration point for pins, types, memory ...       |
+------------------+-------------------------------------------------------+
| CFLAGS           | General compiler flags.                               |
+------------------+-------------------------------------------------------+
| NATCFLAGS        | Flags for the native compiler.                        |
|                  |                                                       |
|                  | The VM is compiled each time in two versions: for     |
|                  | Arduino and for your system, where you compile it. So |
|                  | you will get a command ./retro.nat, which emulates    |
|                  | the environment on Arduino, and the retro.hex for     |
|                  | upload.                                               |
+------------------+-------------------------------------------------------+
| AVRCFLAGS        | Flags for the AVR compiler.                           |
|                  |                                                       |
|                  | This flags are for the GCC AVR cross compiler. It     |
|                  | should contain the CPU type (normaly generated from   |
|                  | BOARD variable) and CPU speed.                        |
+------------------+-------------------------------------------------------+
| IMAGE_BLOCK_SIZE | Size of blocks, in which the image is splitted in     |
|                  | program memory, use 9600 to not to split the image.   |
|                  |                                                       |
|                  | If the IMAGE_MODE is roflash, then the image is       |
|                  | stored as integer array and under some configurations |
|                  | is this array larger as allowed, so it is stored as   |
|                  | several arrays of IMAGE_BLOCK_SIZE size.              |
+------------------+-------------------------------------------------------+
| ROOT_DIRECTORY   | The directory of Retro package ("../../../..")        |
+------------------+-------------------------------------------------------+
| LIBRARY_PATH     | Path to Retro packages ("$ROOT_DIRECTORY/library")    |
+------------------+-------------------------------------------------------+
| MODULES          | List of Retro modules, which should be loaded         |
+------------------+-------------------------------------------------------+
| CC               | Path to the native GCC compiler                       |
+------------------+-------------------------------------------------------+
| AVRGCC           | Path to the AVR GCC cross compiler                    |
+------------------+-------------------------------------------------------+
| AVROBJCOPY       | Path to the AVR version of the "objcopy" command      |
+------------------+-------------------------------------------------------+

Here is the main subset of configurations, which should be included in FLAGS
variable as "-D<VAR>=<VALUE>", e.g. "-DBAUD=19200":

+---------------------+----------------------------------------------------+
| Variable name       | Description                                        |
+=====================+====================================================+
| IMAGE_MODE          | Chooses the mode of the image, following are       |
|                     | supported:                                         |
|                     |                                                    |
|                     | +-----------+------------------------------------+ |
|                     | | Mode      | Description                        | |
|                     | +===========+====================================+ |
|                     | | roflash   | Stores the image readonly in PGM   | |
|                     | +-----------+------------------------------------+ |
|                     | | rwstorage | Uses the some storage as read/write| |
|                     | |           | read/write image. Normally SD card | |
|                     | |           | or similar, which need to semilar, | |
|                     | |           | activated separately.              | |
|                     | +-----------+------------------------------------+ |
+---------------------+----------------------------------------------------+
| BAUD                | Speed of the console connection.                   |
+---------------------+----------------------------------------------------+
| CELL                | Type of the cells in Retro, (uint16_t or uint32_t).|
+---------------------+----------------------------------------------------+
| IMAGE_SIZE          | Theoretical image size, e.g. 320000.               |
+---------------------+----------------------------------------------------+
| IMAGE_CACHE_SIZE    | Size of cache for the image, if the image is in    |
|                     | readonly mode, than all changes are stored in the  |
|                     | cache and newer removed from it, in this case the  |
|                     | cache defines also the maximal number of changed   |
|                     | cells in the image.                                |
+---------------------+----------------------------------------------------+
| CELL_CACHE_TYPE     | Type of pointers in cache, if the cache is smaller |
|                     | 254, then uint8_t otherwise uint16_t.              |
+---------------------+----------------------------------------------------+
| STACK_DEPTH         | Size of the Retro stack.                           |
+---------------------+----------------------------------------------------+
| ADDRESSES           | Size of the return stack.                          |
+---------------------+----------------------------------------------------+
| PORTS               | Number of ports, normally 15.                      |
+---------------------+----------------------------------------------------+
| STRING_BUFFER_SIZE  | Size of string buffer, now is used only if display |
|                     | is activated.                                      |
+---------------------+----------------------------------------------------+
| SPI_PORT            | The pins of the SPI interface.                     |
| SPI_DDR             |                                                    |
| SPI_MISO            |                                                    |
| SPI_MOSI            |                                                    |
| SPI_SCK             |                                                    |
| SPI_SS              |                                                    |
+---------------------+----------------------------------------------------+
| DISPLAY_PORT        | The pins of the display.                           |
| DISPLAY_DDR         |                                                    |
| DISPLAY_SS          |                                                    |
| DISPLAY_DC          |                                                    |
| DISPLAY_RST         |                                                    |
+---------------------+----------------------------------------------------+
| SDCARD_PORT         | The pins of the SD card slot.                      |
| SDCARD_DDR          |                                                    |
| SDCARD_SS           |                                                    |
+---------------------+----------------------------------------------------+
| DISPLAY             | Display type, the only supported now: nokia3110    |
+---------------------+----------------------------------------------------+

To compile the VM simply call the configuration script, the VM file for upload
to the board is retro.hex, if the image type is not "roflash", then you will get
image.data file as the image, which you need to put to the apropriate place.
With following or similar command you can upload the VM to the board:

::

    avrdude -F -c stk500v2 -p m2560 -b 19200 -P /dev/ttyU0 -U flash:w:retro.hex

---------
Footnotes
---------

.. [1] Read more about this project here: http://arduino.cc

.. [2] A concatenative, stack-based programming language with roots in Forth:
       http://retroforth.org
