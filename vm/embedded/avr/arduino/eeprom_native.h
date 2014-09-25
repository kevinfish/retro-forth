#ifndef EEPROM_NATIVE_H
#define EEPROM_NATIVE_H
#ifdef EEPROM_ACTIVATED
#error "Only one EEPROM can be activated."
#endif
#define EEPROM_ACTIVATED

static CELL load_from_eeprom() {
    CELL ret, key, value;
    char *fname = getenv("EEPROM_FILE");
    FILE *f = fopen(fname == NULL ? "eeprom.data" : fname, "r");
    if (f == NULL) goto read_error;
    if (fread((void*)&ret, sizeof(CELL), 1, f) != 1) goto read_error;
    for (int i = 0; i < ret; ++i) {
        if (fread((void*)&key, sizeof(CELL), 1, f) != 1) goto read_error;
        if (fread((void*)&value, sizeof(CELL), 1, f) != 1) goto read_error;
        img_put(key, value);
        console_putc('.');
    }
    fclose(f);
    return ret;
read_error:
    console_puts("\nERROR: failed to read from EEPROM. ");
    if (f != NULL) fclose(f);
    return 0;
}

static CELL save_to_eeprom() {
    CELL ret = 0, x, y, k, v;
    char *fname = getenv("EEPROM_FILE");
    FILE *f = fopen(fname == NULL ? "eeprom.data" : fname, "r+");
    if (f == NULL) goto write_error;
    if (fwrite((void*)&ret, sizeof(CELL), 1, f) != 1) goto write_error;
    for (cell_cache_pointer_t i = 0; i < IMAGE_CACHE_SIZE; ++i) {
        k = cell_cache_keys[i];
        v = cell_cache_values[i];
        if (fwrite(&k, sizeof(CELL), 1, f) != 1) goto write_error;
        if (fwrite(&v, sizeof(CELL), 1, f) != 1) goto write_error;
        if (fseek(f, 0 - 2 * sizeof(CELL), SEEK_CUR) != 0) goto write_error;
        if (fread(&x, sizeof(CELL), 1, f) != 1) goto write_error;
        if (fread(&y, sizeof(CELL), 1, f) != 1) goto write_error;
        if (k != x || v != y) goto write_error;
        ret += 1;
        console_putc('.');
    }
    if (fseek(f, 0, SEEK_SET) != 0) goto write_error;
    if (fwrite((void*)&ret, sizeof(CELL), 1, f) != 1) goto write_error;
    if (fseek(f, 0, SEEK_SET) != 0) goto write_error;
    if (fread((void*)&x, sizeof(CELL), 1, f) != 1) goto write_error;
    fclose(f); f = NULL;
    if (x != ret) goto write_error;
    return ret;
write_error:
    console_puts("\nERROR: failed to write to EEPROM. ");
    if (f != NULL) fclose(f);
    return 0;
}

#endif
