#ifndef EEPROM_ATMEL_H
#define EEPROM_ATMEL_H
#ifdef EEPROM_ACTIVATED
#error "Only one EEPROM can be activated."
#endif
#define EEPROM_ACTIVATED

#include <avr/eeprom.h>

static CELL load_from_eeprom() {
    CELL ret, key, value;
    uint16_t *a;
    ret = eeprom_read_word(0);
    if (ret <= 0) goto read_error;
    for (int i = 0, k = 1; i < ret; ++i, k += 4) {
        a = (uint16_t*)(k+0); key = eeprom_read_word(a);
        a = (uint16_t*)(k+2); value = eeprom_read_word(a);
        img_put(key, value);
        console_putc('.');
    }
    return ret;
read_error:
    console_pute(ERR_READ_EEPROM);
    return 0;
}

static CELL save_to_eeprom() {
    CELL ret = 0, x, y, k, v;
    uint16_t *a;
    eeprom_write_word(0, ret);
    if (eeprom_read_word(0) != 0) goto write_error;
    for (cell_cache_pointer_t i = 0; i < IMAGE_CACHE_SIZE; ++i) {
        k = cell_cache_keys[i];
        v = cell_cache_values[i];
        a = (uint16_t*)(i*4+1); eeprom_write_word(a, k); x = eeprom_read_word(a);
        a = (uint16_t*)(i*4+3); eeprom_write_word(a, v); y = eeprom_read_word(a);
        if (k != x || v != y) goto write_error;
        ret += 1;
        console_putc('.');
    }
    eeprom_write_word(0, ret);
    x = eeprom_read_word(0);
    if (x != ret) goto write_error;
    return ret;
write_error:
    console_pute(ERR_WRITE_EEPROM);
    return 0;
}

#endif
