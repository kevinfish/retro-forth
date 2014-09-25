// This file:
//   http://angg.twu.net/RETRO/luaretro.c.html
//   http://angg.twu.net/RETRO/luaretro.c
// See:
//   http://angg.twu.net/RETRO/README.html
//   http://angg.twu.net/RETRO/VERSION.html
// Author: Eduardo Ochs <eduardoochs@gmail.com>
// Version: 2011apr17 05:50
// but see above.
//
// (find-angg "peek/peek.c")
// (find-luamanualw3m "#luaL_checkint")
// (find-luamanualw3m "#luaL_checklstring")
// (find-luamanualw3m "#lua_pushinteger")
// (find-luamanualw3m "#lua_pushlstring")
// (find-node "(libc)Function Index" "memcpy")
// (find-node "(libc)Function Index" "* malloc")
// (find-node "(libc)Function Index" "* free")

// Note: the "string evaluation" function - to be done - will probably
// be quite similar to "lua_poke" below, but calling some function in
// Retro instead of memcpy.

// «.low-level-build-.so»	(to "low-level-build-.so")
// «.low-level-lua-test»	(to "low-level-lua-test")
// «.test-makefile»		(to "test-makefile")



#include <stdlib.h>		// (find-man "3 malloc")
#include <string.h>		// (find-man "3 memcpy")
#include <lua.h>
#include <lauxlib.h>
#include <termios.h>

static int lua_peek(lua_State* L) {
  int addr = luaL_checkint(L, 1);
  int len  = luaL_checkint(L, 2);
  lua_pushlstring(L, (void *)addr, len);
  return 1;
}

static int lua_poke(lua_State* L) {
  int addr = luaL_checkint(L, 1);
  int len = 0; const char* straddr = luaL_checklstring(L, 2, &len);
  memcpy((void *)addr, straddr, len);
  return 0;
}

static int lua_malloc(lua_State* L) {
  int len = luaL_checkint(L, 1);
  lua_pushinteger(L, (int)malloc(len));
  return 1;
}

static int lua_free(lua_State* L) {
  int addr = luaL_checkint(L, 1);
  free((void *)addr);
  return 0;
}



// (find-anggfile "RETRO/libretro.c")
// (find-anggfile "RETRO/test.c")

#define CELL            int32_t
#define IMAGE_SIZE      1000000
#define ADDRESSES          1024
#define STACK_DEPTH         128
#define PORTS                32
#define MAX_FILE_NAME      1024
#define MAX_REQUEST_LENGTH 1024
#define MAX_OPEN_FILES        8
#define GLOBAL                "/usr/local/share/retro/retroImage"
#define LOCAL                 "retroImage"

enum vm_opcode {VM_NOP, VM_LIT, VM_DUP, VM_DROP, VM_SWAP, VM_PUSH, VM_POP,
                VM_LOOP, VM_JUMP, VM_RETURN, VM_GT_JUMP, VM_LT_JUMP,
                VM_NE_JUMP,VM_EQ_JUMP, VM_FETCH, VM_STORE, VM_ADD,
                VM_SUB, VM_MUL, VM_DIVMOD, VM_AND, VM_OR, VM_XOR, VM_SHL,
                VM_SHR, VM_ZERO_EXIT, VM_INC, VM_DEC, VM_IN, VM_OUT,
                VM_WAIT };
#define NUM_OPS VM_WAIT + 1

typedef struct {
  CELL sp, rsp, ip;
  CELL data[STACK_DEPTH];
  CELL address[ADDRESSES];
  CELL ports[PORTS];
  FILE *files[MAX_OPEN_FILES];
  FILE *input[MAX_OPEN_FILES];
  CELL isp;
  CELL inputSource;
  char *inputString;
  CELL strIndex;
  CELL image[IMAGE_SIZE];
  CELL shrink, padding;
  int stats[NUM_OPS + 1];
  int max_sp, max_rsp;
  char filename[MAX_FILE_NAME];
  char request[MAX_REQUEST_LENGTH];
  struct termios new_termios, old_termios;
} VM;

void rxWriteConsole(VM *vm, CELL c);
CELL rxReadConsole(VM *vm);
void rxIncludeFile(VM *vm, char *s);
void rxPrepareInput(VM *vm);
void rxPrepareOutput(VM *vm);
void rxRestoreIO(VM *vm);
void rxAddInputSource(VM *vm);
CELL rxLoadImage(VM *vm, char *image);
CELL rxSaveImage(VM *vm, char *image);
void rxDeviceHandler(VM *vm);
void rxProcessOpcode(VM *vm);
void rxDisplayStats(VM *vm);

VM *vm;

static int lua_retro_initialize(lua_State* L) {
  vm = calloc(sizeof(VM), sizeof(char));
  strcpy(vm->filename, "retroImage");
  rxLoadImage(vm, "retroImage");
  rxPrepareInput(vm);
  rxPrepareOutput(vm);
  return 0;
}

static int lua_retro_finish(lua_State* L) {
  rxRestoreIO(vm);
  return 0;
}

static int lua_retro_eval(lua_State* L) {
  rxEvaluateString(vm, lua_tostring(L, -1));
  return 0;
}

static int lua_retro_main(lua_State* L) {
  for (vm->ip = 0; vm->ip < IMAGE_SIZE; vm->ip++)
    rxProcessOpcode(vm);
  return 0;
}


LUALIB_API int luaopen_retro(lua_State *L) {
  lua_register(L, "peek_",   lua_peek);
  lua_register(L, "poke_",   lua_poke);
  lua_register(L, "malloc_", lua_malloc);
  lua_register(L, "free_",   lua_free);
  lua_register(L, "retro_main", lua_retro_main);
  lua_register(L, "retro_eval", lua_retro_eval);
  lua_register(L, "retro_initialize", lua_retro_initialize);
  lua_register(L, "retro_finish", lua_retro_finish);
  return 0;
}



/*
// IGNORE THIS!
// (find-angg "peek/peek-0.0.1-0.rockspec" "build")
// (find-angg "peek/peek-0.0.1-0.rockspec" "test")
// (find-angg "peek/olm-peek")
// (find-angg ".zshrc" "lua")

# «low-level-build-.so»  (to ".low-level-build-.so")
 (eepitch-shell)
 (eepitch-kill)
 (eepitch-shell)
cd ~/RETRO/
gcc -g -O0 -Wall -fPIC -I$LUA51SRC/include -c luaretro.c -o luaretro.o
gcc -g -O0 -Wall -fPIC -DNOMAIN            -c libretro.c -o libretro.o
gcc -shared -o retro.so -L$LUA51SRC/lib libretro.o luaretro.o

# «low-level-lua-test»  (to ".low-level-lua-test")
 (eepitch-lua51)
 (eepitch-kill)
 (eepitch-lua51)
assert(package.loadlib("./retro.so", "luaopen_retro"))()
= retro_main_
retro_main_()

# «test-makefile»  (to ".test-makefile")
 (eepitch-shell)
 (eepitch-kill)
 (eepitch-shell)
# (find-angg "RETRO/Makefile" "libretro.so")
cd ~/RETRO/
make clean
make runtest
make LUA51SRC=$LUA51SRC LUA51=$HOME/bin/lua51 runluatest
make lua51
make runluatest

 (eepitch-shell)
 (eepitch-kill)
 (eepitch-shell)
cd ~/RETRO/
make clean
make tgz
ls -l luaretro.tgz
make runluatest

*/

/*
 * Local Variables:
 * coding:             raw-text-unix
 * ee-anchor-format:   "«%s»"
 * End:
 */
