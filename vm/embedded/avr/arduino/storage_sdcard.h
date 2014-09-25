#ifndef STORAGE_SD_H
#define STORAGE_SD_H
#ifdef STORAGE_ACTIVATED
#error "Only one storage device can be activated."
#endif
#define STORAGE_ACTIVATED

#define STORAGE_SECTOR_SIZE 512
#define STORAGE_SECTOR_SHIFT 9

#if BOARD != native

#define SD_NORM 0
#define SD_BUSY 1
#define SD_DESS 1
#define SD_CONT 0

#define SD_CMD_GO_IDLE_STATE 0
#define SD_CMD_SEND_OP_COND 1
#define SD_CMD_SD_SEND_OP_COND 41
#define SD_CMD_NEXT_ACMD 55
#define SD_CMD_SET_BLOCKLEN 16
#define SD_CMD_READ_SINGLE_BLOCK 17
#define SD_CMD_WRITE_BLOCK 24

static struct {
    union {
        struct {
            unsigned int cmd:6;
            unsigned int bit6:1;
            unsigned int bit7:1;
        } dat;
        unsigned int byte:8;
    } command;
    union {
        struct {
            unsigned int arg4:8;
            unsigned int arg3:8;
            unsigned int arg2:8;
            unsigned int arg1:8;
        } dat;
        uint32_t value:32;
    } argument;
    unsigned int crc:8;
} sdcard_command;

static union {
    struct {
        unsigned int in_idle_state:1;
        unsigned int erase_reset:1;
        unsigned int illegal_command:1;
        unsigned int crc_error:1;
        unsigned int erase_seq_error:1;
        unsigned int address_error:1;
        unsigned int parameter_error:1;
        unsigned int start_bit:1;
    } dat;
    unsigned int byte:8;
} sdcard_response;

#if 0
static void sdcard_print_r1(char *msg)
{
    console_puts(msg);
    console_putc(' ');
    if (sdcard_response.dat.start_bit == 1) console_putc('1'); else console_putc('0');
    if (sdcard_response.dat.parameter_error == 1) console_putc('1'); else console_putc('0');
    if (sdcard_response.dat.address_error == 1) console_putc('1'); else console_putc('0');
    if (sdcard_response.dat.erase_seq_error == 1) console_putc('1'); else console_putc('0');
    if (sdcard_response.dat.crc_error == 1) console_putc('1'); else console_putc('0');
    if (sdcard_response.dat.illegal_command == 1) console_putc('1'); else console_putc('0');
    if (sdcard_response.dat.erase_reset == 1) console_putc('1'); else console_putc('0');
    if (sdcard_response.dat.in_idle_state == 1) console_putc('1'); else console_putc('0');
    console_putc(' ');
}
#endif

static void  sdcard_call_r1(
        uint8_t cmdnum,
        uint32_t val,
        uint8_t busy,
        uint8_t deselect)
{
    /* Select SD card */
    SDCARD_PORT &= ~(1<<SDCARD_SS);

    sdcard_command.command.dat.bit7 = 0;
    sdcard_command.command.dat.bit6 = 1;
    sdcard_command.command.dat.cmd = cmdnum;
    sdcard_command.argument.value = val;

    if (cmdnum == 0) sdcard_command.crc = 0x95;
    else sdcard_command.crc = 0;

    /* send command */
    spi_transfer_byte(sdcard_command.command.byte);
    spi_transfer_byte(sdcard_command.argument.dat.arg1);
    spi_transfer_byte(sdcard_command.argument.dat.arg2);
    spi_transfer_byte(sdcard_command.argument.dat.arg3);
    spi_transfer_byte(sdcard_command.argument.dat.arg4);
    spi_transfer_byte(sdcard_command.crc);

    /* recive response */
    for (unsigned int i = 0; i < 200; ++i) {
        sdcard_response.byte = spi_transfer_byte(0xFF);
        if (sdcard_response.dat.start_bit == 0)
            break;
        _delay_ms(1);
    }

    if (busy && sdcard_response.dat.start_bit == 0) {
        if (sdcard_response.dat.start_bit != 0)
            goto finish;
        while (spi_transfer_byte(0xFF) == 0xFF);
        spi_transfer_byte(0xFF);
    }

finish:
    if (deselect) {
        SDCARD_PORT |= (1<<SDCARD_SS);
        spi_transfer_byte(0xFF);
    }
}

static uint8_t storage_init(void) {
    unsigned int i;

    SDCARD_DDR |= (1<<SDCARD_SS);
    SDCARD_PORT |= (1<<SDCARD_SS);
    for (i = 0; i < 10; i++)
        spi_transfer_byte(0xFF);

    for (;;) {
        sdcard_call_r1(SD_CMD_GO_IDLE_STATE, 0, SD_NORM, SD_DESS);
        if (sdcard_response.byte == 1) break;
    }

    for (;;) {
        /* First try the ACMD41 */
        sdcard_call_r1(SD_CMD_NEXT_ACMD, 0, SD_NORM, SD_DESS);
        sdcard_call_r1(SD_CMD_SD_SEND_OP_COND, 0, SD_NORM, SD_DESS);
        if (sdcard_response.dat.start_bit == 0) {
            if (sdcard_response.byte == 0) break;
            if (sdcard_response.dat.illegal_command == 1) {
                /* SD_CMD_SD_SEND_OP_COND seams to be illigal,
                 * switch to SD_CMD_SEND_OP_COND */
                for (;;) {
                    sdcard_call_r1(SD_CMD_SEND_OP_COND, 0, SD_NORM, SD_DESS);
                    if (sdcard_response.byte == 0) break;
                }
                break;
            }
        }
    }

    /* Set block length to 512 */
    for (;;) {
        sdcard_call_r1(SD_CMD_SET_BLOCKLEN, STORAGE_SECTOR_SIZE, SD_NORM, SD_DESS);
        if (sdcard_response.byte == 0) break;
    }

    //spi_fast_mode();
    return 0;
}

static uint8_t storage_read_sector(unsigned char *data, uint32_t addr) {
    char buf[10];
    unsigned int i;

    /*
    sprintf(buf, "\nR%ld ", addr);
    console_puts(buf);
    */

    /* convert sector number to byte address */
    addr <<= STORAGE_SECTOR_SHIFT;

    /* send read sector command */
    for (;;) {
        sdcard_call_r1(SD_CMD_READ_SINGLE_BLOCK, addr, SD_NORM, SD_CONT);
        if (sdcard_response.byte == 0) break;
    }

    /* send and recive data tocken */
    while (spi_transfer_byte(0xFF) != 0xFE);

    /* recv data */
    for (i = 0; i < STORAGE_SECTOR_SIZE; ++i)
        data[i] = spi_transfer_byte(0xFF);

    /* recv crc (2 bytes) */
    spi_transfer_byte(0xFF);
    spi_transfer_byte(0xFF);

    SDCARD_PORT |= (1<<SDCARD_SS);
    spi_transfer_byte(0xFF);

    /*
    for (i = 0; i < STORAGE_SECTOR_SIZE; ++i) {
        unsigned char a, b;
        a = (data[i] >> 4);
        b = data[i] & 0xF;
        if (a > 9) console_putc('A' - 10 + a);
        else console_putc('0' + a);
        if (b > 9) console_putc('A' - 10 + b);
        else console_putc('0' + b);
    }
    console_putc(' ');
    */
    return 0;
}

static uint8_t storage_write_sector(unsigned char *data, uint32_t addr) {
    //char buf[10];
    unsigned int i;

    /*sprintf(buf, "\nW%ld ", addr);
    console_puts(buf);*/

    /* convert sector number to byte address */
    addr <<= STORAGE_SECTOR_SHIFT;

    /* send read sector command */
    for (;;) {
        sdcard_call_r1(SD_CMD_WRITE_BLOCK, addr, SD_NORM, SD_CONT);
        if (sdcard_response.dat.start_bit == 0) break;
    }

    /* send data token */
    spi_transfer_byte(0xFE);

    /* send data */
    for (i = 0; i < 512; ++i)
        spi_transfer_byte(data[i]);

    /* send crc */
    spi_transfer_byte(0);
    spi_transfer_byte(0);

    /* wait while write finished */
    while(spi_transfer_byte(0xFF) == 0);

    SDCARD_PORT |= (1<<SDCARD_SS);
    spi_transfer_byte(0xFF);
    return 0;
}

#else
/* We have native compilation */

#include <unistd.h>

#define STORAGE_IMAGE_FILE "retroSD2"

static FILE *fd;

static CELL check_data[32000];

static uint8_t storage_init() {
    fd = fopen(STORAGE_IMAGE_FILE, "r+");
    fread(check_data, sizeof(CELL), 32000, fd);
    fseek(fd, 0, SEEK_SET);
    return 0;
}

static uint8_t storage_read_sector(uint8_t *buffer, uint32_t sector) {
    char buf[10];
    if (0 != fseek(fd, sector * STORAGE_SECTOR_SIZE, SEEK_SET)) {
        perror("fseek");
        return 1;
    }
    for (int i = 0; i < STORAGE_SECTOR_SIZE; ++i)
        buffer[i] = 0;
    fread(buffer, STORAGE_SECTOR_SIZE, 1, fd);

    /*
    sprintf(buf, "\nR%d ", sector);
    console_puts(buf);
    for (unsigned int i = 0; i < STORAGE_SECTOR_SIZE; ++i) {
        unsigned char a, b;
        a = (buffer[i] >> 4);
        b = buffer[i] & 0xF;
        if (a > 9) console_putc('A' - 10 + a);
        else console_putc('0' + a);
        if (b > 9) console_putc('A' - 10 + b);
        else console_putc('0' + b);
    }*/
    return 0;
}

static uint8_t storage_write_sector(uint8_t *buffer, uint32_t sector) {
    if (0 != fseek(fd, sector * STORAGE_SECTOR_SIZE, SEEK_SET)) {
        perror("fwrite");
        return 1;
    }
    if (1 != fwrite(buffer, STORAGE_SECTOR_SIZE, 1, fd)) {
        perror("fwrite");
        return 2;
    }
    fsync(fileno(fd));
    return 0;
}

#endif

#endif
