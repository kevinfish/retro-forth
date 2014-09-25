#!/bin/sh

err () {
  [ -e error.log ] && cat error.log
  rm -f error.log
  echo "ERROR: $*" >&2
  exit 1
}

[ X"$BOARD" = X"" ] && err "build_binaries.sh should be included in configuration."

echo "Build image file"
cp $ROOT_DIRECTORY/retroImage retroImage || err "Failed to copy retroImage file"
make -C $ROOT_DIRECTORY retro >error.log 2>&1 || err "Failed to builed original retro"
cat $MODULES | $ROOT_DIRECTORY/retro --shrink --image retroImage >retro.log 2>&1 || err "Failed to build retro image"
mv retro.log image.log
tail image.log
$CC $CFLAGS -o convert $ROOT_DIRECTORY/tools/convert.c >error.log 2>&1 || err "Failed to compile convert tool"
./convert >error.log 2>&1 || err "Failed to convert images"
rm -f error.log
ls -l retroImage retroImage16

echo "Create image header"
hexdump -v -e "\"{\" $IMAGE_BLOCK_SIZE/2 \"%d,\" \"},\n\"" retroImage16 \
| sed -e 's/,,* *},/}/' \
| awk "BEGIN { curr = 0; }
{ print \"static const prog_int16_t image_\" curr \"[$IMAGE_BLOCK_SIZE] PROGMEM = \" \$0 \";\";
  curr++;
}
END {
  cell_count = ($IMAGE_BLOCK_SIZE * curr);
  print \"#define IMAGE_CELLS \" cell_count;
  if (curr == 1) {
    print \"#define image_read(x) ((CELL)pgm_read_word(&(image_0[x])))\";
  } else {
    print \"static int16_t image_read(int16_t x) {\";
    print \"  switch(x / ${IMAGE_BLOCK_SIZE}) {\";
    for (i = 0; i < curr; i++) { \
      print \"    case \" i \": return pgm_read_word(&(image_\" i \"[x % $IMAGE_BLOCK_SIZE]));\";
    };
    print \"  } return 0; }\";
  }
}" > image.h \
|| err "Failed to build image header"
ls -l image.h

echo "Build native executable (for testing)"
echo $CC $CFLAGS $NATCFLAGS -o retro.nat retro.c
$CC $CFLAGS $NATCFLAGS -o retro.nat retro.c || err "Failed to build native executable"
rm -f error.log
ls -l retro.nat

echo "Build AVR executable (for uploading)"
echo $AVRCC $CFLAGS $AVRCFLAGS -o retro.avr retro.c
$AVRCC $CFLAGS $AVRCFLAGS -o retro.avr retro.c || err "Failed to build AVR executable"
$AVROBJCOPY -S -O ihex -R .eeprom retro.avr retro.hex >error.log 2>&1 \
|| err "Failed to create upload file"
rm -f error.log
ls -l retro.avr retro.hex

echo "Everything done!"
echo "Examples of upload commands:"
echo "  With ISP> avrdude -V -c usbtiny -p $BOARD -U flash:w:retro.hex"
echo "  Directly> avrdude -V -c stk500v2 -p $BOARD -b 19200 -P /dev/ttyU0 -U flash:w:retro.hex"
echo "Or start it with> ./retro.nat"
