/* Ngaro VM ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Copyright (c) 2008 - 2012, Charles Childers
   Copyright (c) 2009 - 2010, Luke Parrish
   Copyright (c) 2010,        Marc Simpson
   Copyright (c) 2010,        Jay Skeer
   Copyright (c) 2011,        Kenneth Keating
   Copyright (c) 2011,        Erturk Kocalar
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "mbed.h"
#include <stdint.h>

Serial pc(USBTX, USBRX);
LocalFileSystem local("local");

#define CELL          int16_t
#define IMAGE_SIZE      14000
#define ADDRESSES         128
#define STACK_DEPTH        32
#define PORTS              16
#define MAX_FILE_NAME      80
#define MAX_OPEN_FILES      4

CELL sp, rsp, ip;
CELL data[STACK_DEPTH];
CELL address[ADDRESSES];
CELL ports[PORTS];
CELL image[IMAGE_SIZE];
char filename[MAX_FILE_NAME];

/* Macros ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define DROP data[sp] = 0; if (--sp < 0) ip = IMAGE_SIZE;
#define TOS  data[sp]
#define NOS  data[sp-1]
#define TORS address[rsp]

enum opcode {VM_NOP, VM_LIT, VM_DUP, VM_DROP, VM_SWAP, VM_PUSH, VM_POP,
             VM_LOOP, VM_JUMP, VM_RETURN, VM_GT_JUMP, VM_LT_JUMP,
             VM_NE_JUMP,VM_EQ_JUMP, VM_FETCH, VM_STORE, VM_ADD,
             VM_SUB, VM_MUL, VM_DIVMOD, VM_AND, VM_OR, VM_XOR, VM_SHL,
             VM_SHR, VM_ZERO_EXIT, VM_INC, VM_DEC, VM_IN, VM_OUT,
             VM_WAIT };
#define NUM_OPS VM_WAIT + 1

/* Console I/O Support ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
FILE *input[MAX_OPEN_FILES];
CELL isp=0;

void dev_putch(CELL c) {
  if (c > 0) {
    putc((char)c, stdout);
    if (c == '\n') {
      putc('\r', stdout);
    }
  }
  else
    printf("\033[2J\033[1;1H");
  /* Erase the previous character if c = backspace */
  if (c == 8) {
    putc(32, stdout);
    putc(8, stdout);
  }
}

CELL dev_getch() {
  CELL c;
  if ((c = getc(input[isp])) == EOF && input[isp] != stdin) {
    fclose(input[isp--]);
    return 0;
  }
  if (c == EOF && input[isp] == stdin)
    exit(0);
  return c;
}

void dev_include(char *s) {
  FILE *file;
  file = fopen(s, "r");
  if (file)
    input[++isp] = file;
}

void dev_init_input() {
  isp = 0;
  input[isp] = stdin;
}


/* File I/O Support ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
FILE *files[MAX_OPEN_FILES];

CELL file_free()
{
  CELL i;
  for(i = 1; i < MAX_OPEN_FILES; i++)
  {
    if (files[i] == 0)
      return i;
  }
  return 0;
}

void file_add() {
  char s[MAX_FILE_NAME];
  CELL name = TOS; DROP;
  CELL i = 0;
  while(image[name])
    s[i++] = (char)image[name++];
  s[i] = 0;
  dev_include(s);
}

CELL file_handle() {
  CELL slot = file_free();
  CELL mode = TOS; DROP;
  CELL i, address = TOS; DROP;
  for (i = 0; i < MAX_FILE_NAME; i++) {
    filename[i] = image[address+i];
    if (! filename[i]) break;
  }
  if (slot > 0)
  {
    if (mode == 0)  files[slot] = fopen(filename, "r");
    if (mode == 1)  files[slot] = fopen(filename, "w");
    if (mode == 2)  files[slot] = fopen(filename, "a");
    if (mode == 3)  files[slot] = fopen(filename, "r+");
  }
  if (files[slot] == NULL)
  {
    files[slot] = 0;
    slot = 0;
  }
  return slot;
}

CELL file_readc() {
  CELL c = fgetc(files[TOS]); DROP;
  if ( c == EOF )
    return 0;
  else
    return c;
}

CELL file_writec() {
  CELL slot = TOS; DROP;
  CELL c = TOS; DROP;
  CELL r = fputc(c, files[slot]);
  if ( r == EOF )
    return 0;
  else
    return 1;
}

CELL file_closehandle() {
  fclose(files[TOS]);
  files[TOS] = 0;
  DROP;
  return 0;
}

CELL file_getpos() {
  CELL slot = TOS; DROP;
  return (CELL) ftell(files[slot]);
}

CELL file_seek() {
  CELL slot = TOS; DROP;
  CELL pos  = TOS; DROP;
  CELL r = fseek(files[slot], pos, SEEK_SET);
  return r;
}

CELL file_size() {
  CELL slot = TOS; DROP;
  CELL current = ftell(files[slot]);
  CELL r = fseek(files[slot], 0, SEEK_END);
  CELL size = ftell(files[slot]);
  fseek(files[slot], current, SEEK_SET);
  if ( r == 0 )
    return size;
  else
    return 0;
}

CELL file_delete() {
  CELL i, address;
  address = TOS; DROP;
  for (i = 0; i < MAX_FILE_NAME; i++) {
    filename[i] = image[address+i];
    if (! filename[i]) break;
  }
  if (remove(filename) == 0)
    return -1;
  else
    return 0;
}

CELL load_image() {
  FILE *fp;
  CELL x = -1;
  if ((fp = fopen("/local/retroImg", "rb")) != NULL) {
    x = fread(&image, sizeof(CELL), IMAGE_SIZE, fp);
    fclose(fp);
  }
  return x;
}

CELL save_image() {
  FILE *fp;
  CELL x = -1;

  if ((fp = fopen("/local/retroImg", "wb")) == NULL)
  {
    printf("Sorry, but I couldn't write to retroImg\n");
    exit(-1);
  }

  x = fwrite(&image, sizeof(CELL), image[3], fp);
  fclose(fp);

  return x;
}

/* Environment Query ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void dev_getenv() {
  DROP; DROP;
}

/* mbed devices ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  DigitalInOut P5(p5);
  DigitalInOut P6(p6);
  DigitalInOut P7(p7);
  DigitalInOut P8(p8);
  DigitalInOut P9(p9);
  DigitalInOut P10(p10);
  DigitalInOut P11(p11);
  DigitalInOut P12(p12);
  DigitalInOut P13(p13);
  DigitalInOut P14(p14);
  DigitalInOut P15(p15);
  DigitalInOut P16(p16);
  DigitalInOut P17(p17);
  DigitalInOut P18(p18);
  DigitalInOut P19(p19);
  DigitalInOut P20(p20);
  DigitalInOut P21(p21);
  DigitalInOut P22(p22);
  DigitalInOut P23(p23);
  DigitalInOut P24(p24);
  DigitalInOut P25(p25);
  DigitalInOut P26(p26);
  DigitalInOut P27(p27);
  DigitalInOut P28(p28);
  DigitalInOut P29(p29);
  DigitalInOut P30(p30);
  DigitalOut L1(LED1);
  DigitalOut L2(LED2);
  DigitalOut L3(LED3);
  DigitalOut L4(LED4);
void handle_mbed() {
  switch (ports[13]) {
    case -1: switch (TOS) {
               case 5: P5.output(); break;
               case 6: P6.output(); break;
               case 7: P7.output(); break;
               case 8: P8.output(); break;
               case 9: P9.output(); break;
               case 10: P10.output(); break;
               case 11: P11.output(); break;
               case 12: P12.output(); break;
               case 13: P13.output(); break;
               case 14: P14.output(); break;
               case 15: P15.output(); break;
               case 16: P16.output(); break;
               case 17: P17.output(); break;
               case 18: P18.output(); break;
               case 19: P19.output(); break;
               case 20: P20.output(); break;
               case 21: P21.output(); break;
               case 22: P22.output(); break;
               case 23: P23.output(); break;
               case 24: P24.output(); break;
               case 25: P25.output(); break;
               case 26: P26.output(); break;
               case 27: P27.output(); break;
               case 28: P28.output(); break;
               case 29: P29.output(); break;
               case 30: P30.output(); break;
             }
             ports[13] = 0;
             break;
    case -2: switch (TOS) {
               case 5: P5.input(); break;
               case 6: P6.input(); break;
               case 7: P7.input(); break;
               case 8: P8.input(); break;
               case 9: P9.input(); break;
               case 10: P10.input(); break;
               case 11: P11.input(); break;
               case 12: P12.input(); break;
               case 13: P13.input(); break;
               case 14: P14.input(); break;
               case 15: P15.input(); break;
               case 16: P16.input(); break;
               case 17: P17.input(); break;
               case 18: P18.input(); break;
               case 19: P19.input(); break;
               case 20: P20.input(); break;
               case 21: P21.input(); break;
               case 22: P22.input(); break;
               case 23: P23.input(); break;
               case 24: P24.input(); break;
               case 25: P25.input(); break;
               case 26: P26.input(); break;
               case 27: P27.input(); break;
               case 28: P28.input(); break;
               case 29: P29.input(); break;
               case 30: P30.input(); break;
             }
             ports[13] = 0;
             break;
    case  5: P5 = TOS;  DROP; ports[13] = 0; break;
    case  6: P6 = TOS;  DROP; ports[13] = 0; break;
    case  7: P7 = TOS;  DROP; ports[13] = 0; break;
    case  8: P8 = TOS;  DROP; ports[13] = 0; break;
    case  9: P9 = TOS;  DROP; ports[13] = 0; break;
    case 10: P10 = TOS; DROP; ports[13] = 0; break;
    case 11: P11 = TOS; DROP; ports[13] = 0; break;
    case 12: P12 = TOS; DROP; ports[13] = 0; break;
    case 13: P13 = TOS; DROP; ports[13] = 0; break;
    case 14: P14 = TOS; DROP; ports[13] = 0; break;
    case 15: P15 = TOS; DROP; ports[13] = 0; break;
    case 16: P16 = TOS; DROP; ports[13] = 0; break;
    case 17: P17 = TOS; DROP; ports[13] = 0; break;
    case 18: P18 = TOS; DROP; ports[13] = 0; break;
    case 19: P19 = TOS; DROP; ports[13] = 0; break;
    case 20: P20 = TOS; DROP; ports[13] = 0; break;
    case 21: P21 = TOS; DROP; ports[13] = 0; break;
    case 22: P22 = TOS; DROP; ports[13] = 0; break;
    case 23: P23 = TOS; DROP; ports[13] = 0; break;
    case 24: P24 = TOS; DROP; ports[13] = 0; break;
    case 25: P25 = TOS; DROP; ports[13] = 0; break;
    case 26: P26 = TOS; DROP; ports[13] = 0; break;
    case 27: P27 = TOS; DROP; ports[13] = 0; break;
    case 28: P28 = TOS; DROP; ports[13] = 0; break;
    case 29: P29 = TOS; DROP; ports[13] = 0; break;
    case 30: P30 = TOS; DROP; ports[13] = 0; break;
    case 31: L1  = TOS; DROP; ports[13] = 0; break;
    case 32: L2  = TOS; DROP; ports[13] = 0; break;
    case 33: L3  = TOS; DROP; ports[13] = 0; break;
    case 34: L4  = TOS; DROP; ports[13] = 0; break;
    default: ports[13] = 0;
  }
  switch (ports[14]) {
    case  5: sp++; TOS = P5; ports[14] = 0; break;
    case  6: sp++; TOS = P6; ports[14] = 0; break;
    case  7: sp++; TOS = P7; ports[14] = 0; break;
    case  8: sp++; TOS = P8; ports[14] = 0; break;
    case  9: sp++; TOS = P9; ports[14] = 0; break;
    case 10: sp++; TOS = P10; ports[14] = 0; break;
    case 11: sp++; TOS = P11; ports[14] = 0; break;
    case 12: sp++; TOS = P12; ports[14] = 0; break;
    case 13: sp++; TOS = P13; ports[14] = 0; break;
    case 14: sp++; TOS = P14; ports[14] = 0; break;
    case 15: sp++; TOS = P15; ports[14] = 0; break;
    case 16: sp++; TOS = P16; ports[14] = 0; break;
    case 17: sp++; TOS = P17; ports[14] = 0; break;
    case 18: sp++; TOS = P18; ports[14] = 0; break;
    case 19: sp++; TOS = P19; ports[14] = 0; break;
    case 20: sp++; TOS = P20; ports[14] = 0; break;
    case 21: sp++; TOS = P21; ports[14] = 0; break;
    case 22: sp++; TOS = P22; ports[14] = 0; break;
    case 23: sp++; TOS = P23; ports[14] = 0; break;
    case 24: sp++; TOS = P24; ports[14] = 0; break;
    case 25: sp++; TOS = P25; ports[14] = 0; break;
    case 26: sp++; TOS = P26; ports[14] = 0; break;
    case 27: sp++; TOS = P27; ports[14] = 0; break;
    case 28: sp++; TOS = P28; ports[14] = 0; break;
    case 29: sp++; TOS = P29; ports[14] = 0; break;
    case 30: sp++; TOS = P30; ports[14] = 0; break;
    default: ports[14] = 0;
  }
}

/* Device I/O Handler ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void handle_devices() {
  if (ports[0] != 1) {
    /* Input */
    if (ports[0] == 0 && ports[1] == 1) {
      ports[1] = dev_getch();
      ports[0] = 1;
    }

    /* Output (character generator) */
    if (ports[2] == 1) {
      dev_putch(TOS); DROP
      ports[2] = 0;
      ports[0] = 1;
    }

    /* File IO and Image Saving */
    if (ports[4] != 0) {
      ports[0] = 1;
      switch (ports[4]) {
        case  1: save_image();
                 ports[4] = 0;
                 break;
        case  2: file_add();
                 ports[4] = 0;
                 break;
        case -1: ports[4] = file_handle();
                 break;
        case -2: ports[4] = file_readc();
                 break;
        case -3: ports[4] = file_writec();
                 break;
        case -4: ports[4] = file_closehandle();
                 break;
        case -5: ports[4] = file_getpos();
                 break;
        case -6: ports[4] = file_seek();
                 break;
        case -7: ports[4] = file_size();
                 break;
        case -8: ports[4] = file_delete();
                 break;
        default: ports[4] = 0;
      }
    }

    /* Capabilities */
    if (ports[5] != 0) {
      ports[0] = 1;
      switch(ports[5]) {
        case -1:  ports[5] = IMAGE_SIZE;
                  break;
        case -2:  ports[5] = 0;
                  break;
        case -3:  ports[5] = 0;
                  break;
        case -4:  ports[5] = 0;
                  break;
        case -5:  ports[5] = sp;
                  break;
        case -6:  ports[5] = rsp;
                  break;
        case -7:  ports[5] = 0;
                  break;
        case -8:  ports[5] = time(NULL);
                  break;
        case -9:  ports[5] = 0;
                  ip = IMAGE_SIZE;
                  break;
        case -10: ports[5] = 0;
                  dev_getenv();
                  break;
        case -11: ports[5] = 80;
                  break;
        case -12: ports[5] = 25;
                  break;
        default:  ports[5] = 0;
      }
    }

    /* mbed io devices */
    if (ports[13] != 0 || ports[14] != 0) {
      ports[0] = 1;
      handle_mbed();
    }

  }
}

/* The VM ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void init_vm() {
   CELL a;
   ip = 0;  sp = 0;  rsp = 0;
   for (a = 0; a < STACK_DEPTH; a++)
      data[a] = 0;
   for (a = 0; a < ADDRESSES; a++)
      address[a] = 0;
   for (a = 0; a < IMAGE_SIZE; a++)
      image[a] = 0;
   for (a = 0; a < PORTS; a++)
      ports[a] = 0;
}

void process() {
  CELL a, b, opcode;
  opcode = image[ip];

  switch(opcode) {
    case VM_NOP:
         break;
    case VM_LIT:
         sp++;
         ip++;
         TOS = image[ip];
         break;
    case VM_DUP:
         sp++;
         data[sp] = NOS;
         break;
    case VM_DROP:
         DROP
         break;
    case VM_SWAP:
         a = TOS;
         TOS = NOS;
         NOS = a;
         break;
    case VM_PUSH:
         rsp++;
         TORS = TOS;
         DROP
         break;
    case VM_POP:
         sp++;
         TOS = TORS;
         rsp--;
         break;
    case VM_LOOP:
         TOS--;
         if (TOS != 0 && TOS > -1)
         {
           ip++;
           ip = image[ip] - 1;
         }
         else
         {
           ip++;
           DROP;
         }
         break;
    case VM_JUMP:
         ip++;
         ip = image[ip] - 1;
         if (ip < 0)
           ip = IMAGE_SIZE;
         else {
           if (image[ip+1] == 0)
             ip++;
           if (image[ip+1] == 0)
             ip++;
         }
         break;
    case VM_RETURN:
         ip = TORS;
         rsp--;
         if (ip < 0)
           ip = IMAGE_SIZE;
         else {
           if (image[ip+1] == 0)
             ip++;
           if (image[ip+1] == 0)
             ip++;
         }
         break;
    case VM_GT_JUMP:
         ip++;
         if(NOS > TOS)
           ip = image[ip] - 1;
         DROP DROP
         break;
    case VM_LT_JUMP:
         ip++;
         if(NOS < TOS)
           ip = image[ip] - 1;
         DROP DROP
         break;
    case VM_NE_JUMP:
         ip++;
         if(TOS != NOS)
           ip = image[ip] - 1;
         DROP DROP
         break;
    case VM_EQ_JUMP:
         ip++;
         if(TOS == NOS)
           ip = image[ip] - 1;
         DROP DROP
         break;
    case VM_FETCH:
         TOS = image[TOS];
         break;
    case VM_STORE:
         image[TOS] = NOS;
         DROP DROP
         break;
    case VM_ADD:
         NOS += TOS;
         DROP
         break;
    case VM_SUB:
         NOS -= TOS;
         DROP
         break;
    case VM_MUL:
         NOS *= TOS;
         DROP
         break;
    case VM_DIVMOD:
         a = TOS;
         b = NOS;
         TOS = b / a;
         NOS = b % a;
         break;
    case VM_AND:
         a = TOS;
         b = NOS;
         DROP
         TOS = a & b;
         break;
    case VM_OR:
         a = TOS;
         b = NOS;
         DROP
         TOS = a | b;
         break;
    case VM_XOR:
         a = TOS;
         b = NOS;
         DROP
         TOS = a ^ b;
         break;
    case VM_SHL:
         a = TOS;
         b = NOS;
         DROP
         TOS = b << a;
         break;
    case VM_SHR:
         a = TOS;
         b = NOS;
         DROP
         TOS = b >>= a;
         break;
    case VM_ZERO_EXIT:
         if (TOS == 0) {
           DROP
           ip = TORS;
           rsp--;
         }
         break;
    case VM_INC:
         TOS += 1;
         break;
    case VM_DEC:
         TOS -= 1;
         break;
    case VM_IN:
         a = TOS;
         TOS = ports[a];
         ports[a] = 0;
         break;
    case VM_OUT:
         ports[0] = 0;
         ports[TOS] = NOS;
         DROP DROP
         break;
    case VM_WAIT:
         handle_devices();
         break;
    default:
         rsp++;
         TORS = ip;
         ip = image[ip] - 1;

         if (ip < 0)
           ip = IMAGE_SIZE;
         else {
           if (image[ip+1] == 0)
             ip++;
           if (image[ip+1] == 0)
             ip++;
         }
         break;
  }
  ports[3] = 1;
}

/* Main ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int main(int argc, char **argv)
{
  while (1) {
    init_vm();
    dev_init_input();

    if (load_image() == -1) {
      printf("Sorry, unable to find retroImg\n");
      exit(1);
    }

    for (ip = 0; ip < IMAGE_SIZE; ip++)
      process();
  }
}

