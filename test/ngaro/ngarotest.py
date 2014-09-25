"""
external test suite for ngaro implementations
-----------------------------------------------
Copyright (c) 2012 Michal J Wallace
Available to the public under the ISC license.
-----------------------------------------------
This test suite depends on implementation of a
switch, --dump, which dumps the internal state
of the interpreter to stdout when the vm exits,
in the following format:

 chr( 28 )  # ascii file separator
   ' '.join( str( i ) for i in data_stack )
 chr( 29 ) # ascii group separator
   ' '.join( str( i ) for i in addr_stack )
 chr( 29 )
   ' '.join( str( i ) for i in ram )

By default, the tests expect the ngaro interpreter
to be invoked with the command "./retro". To override
this, set the NGARO_PATH environment variable.

For example, in bash:

  export NGARO_CMD='python /path/to/retro.py'

The command invoked for each test will then be:

  python /path/to/retro.py --dump --image ngarotest.img

"""
import unittest, os, array, sys, subprocess
NGARO_CMD = os.environ.get( "NGARO_CMD", './ngaro.cmd' )
NGARO_IMG = os.environ.get( "NGARO_IMG", './ngarotest.img' )


# opcode list and helper functions

ops = {
  "nop"   : 0,   "lit"   : 1,   "dup"   : 2,    "drop"  : 3,
  "swap"  : 4,   "push"  : 5,   "pop"   : 6,    "loop"  : 7,
  "jump"  : 8,   ";"     : 9,   "<jump" : 10,   ">jump" : 11,
  "!jump" : 12,  "=jump" : 13,  "@"     : 14,   "!"     : 15 ,
  "+"     : 16,  "-"     : 17,  "*"     : 18,   "/mod"  : 19,
  "and"   : 20,  "or"    : 21,  "xor"   : 22,   "<<"    : 23,
  ">>"    : 24,  "0;"    : 25,  "1+"    : 26,   "1-"    : 27,
  "in"    : 28,  "out"   : 29,  "wait"  : 30,
}


def is_int( x ):
  res = True
  try: int( x )
  except ValueError: res = False
  return res


def trim( s ):
  """
  strips leading indentation from a multi-line string.
  for saving bandwidth while making code look nice
  """
  lines = string.split( s, "\n" )
  # strip leading blank line
  if lines[0] == "": lines = lines[ 1: ]
  # strip indentation
  indent = len( lines[ 0 ]) - len( lines[ 0 ].lstrip( ))
  for i in range( len( lines )): lines[ i ] = lines[ i ][ indent: ]
  return '\n'.join( lines )


# assembler

def assemble( src ):
  """
  :: str -> [ int ] or raise ValueError
  --- example program ------------------

    # count until stack overflows
      0           # emits 1 0   ( 1 is an implicit 'lit' opcode )
    :COUNTER
      dup
      inc
      jump COUNTER

  --- end example ----------------------
  """
  labels = {}
  res  = []
  # we do two passes so that we can use labels before they're defined
  for phase in [ 1 , 2 ]:
    here = 0
    for ln, line in enumerate( src.split( "\n" )):
      for word in line.split( ):
        if word.startswith( "#" ): break
        if word.startswith( ":" ): labels[ word[ 1: ]] = here
        else:
          here += 1
          if is_int( word ) :
            here += 1 # add another because of the implicit 'lit' op
            if phase == 2 : res.extend([ ops[ 'lit' ], int( word ) ])
          # the rest are all emitters and we only do that in phase 2:
          elif phase == 1 : pass
          elif word in labels : res.append( labels[ word ])
          elif word in ops : res.append( ops[ word ])
          else : raise ValueError( "unresolved symbol '%s' on line %i"
                                   % ( word, ln + 1 ))
  return res


# save_image ( and 32-bit int encoding )

INT32_FMT = ''
for ch in 'hil':
  if array.array( ch ).itemsize == 4: INT32_FMT = ch
if not INT32_FMT:
  print "couldn't find a 32-bit int in your python's array module."
  print "sorry -- you'll need to update the save_image function or"
  print "use a differnt python version."
  print
  print "see http://docs.python.org/library/array.html#module-array"
  sys.exit()


def save_image( ints, path ):
  """
  write a list of ints as 32-bit binary values
  """
  assert len( ints ), "no instructions given"
  arr = array.array( INT32_FMT, ints )
  img = open( path, 'wb' )
  arr.tofile( img )
  img.close( )



# show control codes

ESC = chr( 27 )
USE_COLOR = True

def maybecolor( string, fg=5, bg=0 ):
  # http://en.wikipedia.org/wiki/ANSI_escape_code#Colors
  if USE_COLOR: return "\x1B[0;3%i;4%im%s\x1B[0m" % ( fg, bg, string )
  else: return string


def catv( string, prefix='^' ):
  """show control characters, like unix cat -v"""
  result = []
  for ch in string:
    if ord( ch ) < 32 and ch not in "\t\n":
      result.append( maybecolor( prefix + '@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_'[ ord( ch )]))
    else: result.append( ch )
  return ''.join( result )

# run

FSEP, GSEP = chr( 28 ), chr( 29 ) # ascii file and group separators

def run( src, imgpath = NGARO_IMG ):
  """
  run the ngaro interpreter and investigate its state.
  see module docstring at top of file for dump format
  """

  save_image( assemble( src ), imgpath )

  cmd = "%s --dump --image %s" % ( NGARO_CMD, imgpath )
  sp = subprocess # module
  child = sp.Popen( cmd, shell=True, stdin=sp.PIPE, stdout=sp.PIPE,
                    stderr=sp.STDOUT, close_fds=True )

  output = child.stdout.read( )
  assert FSEP in output, \
      "\n\nNo state dump found.\nIssued: %s\nOutput:\n%s" % ( cmd, catv( output ))

  rest, dump = output.rsplit( FSEP, 1 )
  data, addr, ram = dump.split( GSEP )

  class res : pass
  res.data = map( int, data.split( ))
  res.addr = map( int, addr.split( ))
  res.ram = map( int, ram.split( ))
  res.out = rest
  return res

# test suite

class AssemblerTest( unittest.TestCase ):
  """self-tests for the assembler"""
  def test_labels( self ):
    self.assertEquals( [ 0 ], assemble( ':zero zero' ))
    self.assertEquals( [ 1 ], assemble( 'one :one' ))
  def test_literal( self ):
    """make sure literals increment label counter by 2"""
    self.assertEquals( [ 0, 1, 2, 3 ], assemble( ':zero zero 2 :three three' ))

class NgaroTests( unittest.TestCase ):

  def tearDown( self ): # runs after each test
    if os.path.exists( NGARO_IMG ):
      os.unlink( NGARO_IMG )

  def test_NOP( self ): # nop
    vm = run( 'nop', 'nop.img' )
    self.assertEquals( [ ], vm.data )
    self.assertEquals( [ ], vm.addr )
    self.assertEquals( [ 0 ], vm.ram )


# stack operations

  def test_LIT( self ): # lit
    vm = run( '25 ', 'lit.img' )
    self.assertEquals( [ 25 ], vm.data )
    self.assertEquals( [ ], vm.addr )
    self.assertEquals( [ 1, 25 ], vm.ram )


  def test_DUP( self ): # dup
    vm = run( '1234 dup', 'dup.img' )
    self.assertEquals( [ 1234, 1234 ], vm.data )
    self.assertEquals( [ ], vm.addr )


  def test_DROP( self ): # drop
    vm = run( '50 drop', 'drop.img' )
    self.assertEquals( [ ], vm.data )
    self.assertEquals( [ ], vm.addr )


  def test_SWAP( self ): # swap
    vm = run( '50 60 swap', 'swap.img' )
    self.assertEquals( [ 60, 50 ], vm.data )
    self.assertEquals( [ ], vm.addr )


  def test_PUSH( self ): # push
    vm = run( '82 push', 'push.img' )
    self.assertEquals( [ ], vm.data )
    self.assertEquals( [ 82 ], vm.addr )


  def test_POP( self ): # pop
    vm = run( '82 push pop', 'pop.img' )
    self.assertEquals( [ 82 ], vm.data )
    self.assertEquals( [ ], vm.addr )



# flow control, part 1 : loop, gosub, return, conditional return

  def nops( self, count ):
    return " nop " * count


  def test_LOOP( self ): # loop
    """
    LOOP should work like this:
      dec( tos )
      if tos > 0 : jump to address in next cell.
      else: drop
    """
    vm = run(" 3 :REPEAT dup push loop REPEAT ", "loop.img")
    self.assertEquals( [ ], vm.data )
    self.assertEquals( [ 3, 2, 1 ], vm.addr )


  def test_gosub( self ): # opcodes above 30
    vm = run(" sub " + self.nops( 32 ) +  ":sub 1 2", "gosub.img" )
    self.assertEquals([ 1, 2 ], vm.data)
    self.assertEquals([ 0 ], vm.addr )


  def test_RETURN( self ): # ;
    vm = run(" sub end " + self.nops( 32 ) +  ":sub -2 :end -1", "return.img" )
    self.assertEquals([ -2, -1 ], vm.data)
    self.assertEquals([ 0 ], vm.addr )


  def test_ZERO_EXIT( self ): # 0;
    vm = run(" fallthrough return quit " + self.nops( 32 ) +
             " :fallthrough 0 1 0; " +
             " :return     -1 0 0; " +
             " :quit ",  "zex.img" )
    self.assertEquals([ 0, 1, -1,      # from call to :fallthrough
                       -1 ], vm.data ) # from call to :return
    self.assertEquals([ 2 ], vm.addr ) # from call to :quit


# flow control, part 2 -- jumps

  def test_JUMP( self ): # jump
    vm = run(" 1 2 jump OVER 3 4 5 :OVER 6 7 ", "jump.img" )
    self.assertEquals([ 1, 2, 6, 7 ], vm.data )
    self.assertEquals([ ], vm.addr )

# for the four conditional jumps, the test format is :
# < test that doesn't jump > < test that does jump >

  def test_LT_JUMP( self ): # <jump
    vm = run(" 1 2 <jump END 12 2 1 <jump END 21 :END", "lt_jump.img" )
    self.assertEquals([ 12 ], vm.data )
    self.assertEquals([ ], vm.addr )


  def test_GT_JUMP( self ): # >jump
    vm = run(" 2 1 >jump END 21 1 2 >jump END 12 :END", "gt_jump.img" )
    self.assertEquals([ 21 ], vm.data )
    self.assertEquals([ ], vm.addr )


  def test_NE_JUMP( self ): # !jump
    vm = run(" 1 1 !jump END 11 1 0 !jump END 10 :END", "gt_jump.img" )
    self.assertEquals([ 11 ], vm.data )
    self.assertEquals([ ], vm.addr )


  def test_EQ_JUMP( self ): # =jump
    vm = run(" 1 0 =jump END 10 1 1 =jump END 11 :END", "gt_jump.img" )
    self.assertEquals([ 10 ], vm.data )
    self.assertEquals([ ], vm.addr )


# arithmetic operations

  def test_ADD( self ): # +
    vm = run( " 2 3 +    2 -3 + ", "add.img" )
    self.assertEquals([ 5, -1 ], vm.data )
    self.assertEquals([ ], vm.addr )


  def test_SUBTRACT( self ): # -
    vm = run( " 2 1 -   1 2 -   1 -2 -   -1 -2 - ", "subtract.img" )
    self.assertEquals([ 1, -1, 3, 1 ], vm.data )
    self.assertEquals([ ], vm.addr )


  def test_MULTIPLY( self ): # *
    vm = run( " 0 5 *   1 5 *   5 5 * ", "multiply.img" )
    self.assertEquals([ 0, 5, 25 ], vm.data )
    self.assertEquals([ ], vm.addr )


  def test_DIVMOD( self ): # /mod
    vm = run( " 25 5 /mod  26 5 /mod  ", "divmod.img" )
    self.assertEquals([ 0, 5, 1, 5 ], vm.data )
    self.assertEquals([ ], vm.addr )


  def test_INC( self ): # 1+
    vm = run( " -1 1+   0 1+    1 1+ ", "inc.img" )
    self.assertEquals([ 0, 1, 2  ], vm.data )
    self.assertEquals([ ], vm.addr )


  def test_DEC( self ): # 1-
    vm = run( " -1 1-    0 1-   1 1- ", "dec.img" )
    self.assertEquals([ -2, -1, 0  ], vm.data )
    self.assertEquals([ ], vm.addr )


# logical / bitwise operations

  def test_AND( self ): # and
    vm = run( " 0 0 and  0 1 and   1 0 and  1 1 and ", "and.img" )
    self.assertEquals([ 0, 0, 0, 1 ], vm.data )
    self.assertEquals([ ], vm.addr )


  def test_OR( self ): # or
    vm = run( " 0 0 or   0 1 or   1 0 or   1 1 or ", "or.img" )
    self.assertEquals([ 0, 1, 1, 1 ], vm.data )
    self.assertEquals([ ], vm.addr )


  def test_XOR( self ): # xor
    vm = run( " 0 0 xor   0 1 xor   1 0 xor   1 1 xor   -1 3 xor ", "xor.img" )
    self.assertEquals([ 0, 1, 1, 0, -4 ], vm.data )
    self.assertEquals([ ], vm.addr )

# note on shifts: behavior on negative shift is undefined, so untested

  def test_SHL( self ): # <<
    vm = run( " 1 1 <<   2 1 <<   3 1 <<   0 2 <<   -1 2 <<  -3 4 << ", "shl.img" )
    self.assertEquals([ 2, 4, 6, 0, -4, -48 ], vm.data )
    self.assertEquals([ ], vm.addr )


  def test_SHR( self ): # >>
    vm = run( " 2 1 >>   4 1 >>   6 1 >>   0 2 >>   -4 2 >>   -48 4 >> ", "shr.img" )
    self.assertEquals([ 1, 2, 3, 0, -1, -3 ], vm.data )
    self.assertEquals([ ], vm.addr )

# memory and I/O operations

  def test_FETCH( self ): # @
    vm = run( " 1234 drop   0 @   1 @ ", "fetch.img" )
    self.assertEquals([ 1, 1234 ], vm.data )
    self.assertEquals([ ], vm.addr )
    

  def test_STORE( self ): # !
    vm = run( " nop   5432 0 ! ", "store.img" )
    self.assertEquals([ ], vm.data )
    self.assertEquals([ ], vm.addr )
    self.assertEquals([ 5432, 1, 5432, 1, 0, ops['!'] ], vm.ram )


  def test_IOW( self ): # in, out, wait
    """
    the i/o stuff is a little harder to test in isolation,
    without adding even more hooks into each vm to allow
    reading and writing to the ports.

    Instead, this just does all three at once, and leaves
    the more complicated testing stuff to /test/*.rx
    """
    vm = run(
      """
      -13 5 out wait  # vm query : bits per cell?
          5 in  wait
      """, "iow.img" )

    # we know the vm is 32 bits because that's what how we're
    # writing the images. The docs say that the answer to
    # the query can be either 32 or 0
    assert vm.data == [ 32 ] or vm.data == [ 0 ], \
            "expected [ 0 ] or [ 32 ], got %r" % vm.data


if __name__=="__main__":
  i = 1
  while i < len( sys.argv ):
    arg = sys.argv[ i ]
    if arg == '-n':
      NGARO_CMD = sys.argv[ i + 1 ]
      sys.argv.remove( arg )
      sys.argv.remove( NGARO_CMD )
    else:
      # unittest kludge to allow testing a single opcode by name
      name = 'test_' + arg.upper()
      if hasattr( NgaroTests, name ):
        sys.argv[ i ] = 'NgaroTests.' + name
      i += 1

  print
  print "NGARO_CMD = %s" % NGARO_CMD
  unittest.main()
