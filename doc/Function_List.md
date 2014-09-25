<link href='http://kevinburke.bitbucket.org/markdowncss/markdown.css' rel='stylesheet'></link>
# Global Functions

## help

    ( - )

shows some helpful words and what they do

## libraries

    ( - )

Display a list of all vocabularies available to load

## doc

    ( ''- )

fetch a token and show documentation for it

## exists?

    ( a-af )

true if the address &lt;&gt; 0

## chwords

    ( ``- )

parse for a chain and show all words inside 

## chains
undocumented

## see
undocumented

## matching
undocumented

## :doc

    ( $- )

assign documentation to last defined function

## vt'
undocumented

## dissect'
undocumented

## describe

    ( ``- )

provide information about a function

## types'

    ( - )

vocabulary with constants for data types. Used with **each@**

## needs

    ( ``- )

Load a vocabulary from the *library* if it is not already loaded (parsing)

## :needs

    ( $- )

Load a vocabulary from the library if not already loaded

## __^

    ( ``- )

Allow direct access to functions in a chain

## strings'

    ( - )

vocabulary with functions for dealing with strings

## buffer'

    ( - )

vocabulary for dealing with LIFO buffers

## internals'

    ( - )

vocabulary containing functions used internally by Retro

## variables|

    ( ``- )

Create a series of variables

## doc{

    ( ``- )

Parse tokens up to *}doc* and ignore.

This is intended as a means of embedding docs into libraries.

## yield

    ( - )

Return from a function, with execution resuming from point after **yield** when the  function is next called

## later

    ( - )

Defer execution of caller until a later time

## getEnv

    ( a$- )

Get a copy of environment variable $ in buffer

## delay

    ( n- )

Delay for (approximately) n seconds

## time

    ( -n )

Return the current unix time

## include

    ( ``- )

Same as **:include**, but parse for file name

## :include

    ( $- )

Include a file

## getNumber

    ( ``-n )

Read a number from the input stream

## getToken

    ( ``-$ )

Read a string, stopping at first whitespace

## bye

    ( - )

Exit Retro

## save

    ( - )

Save the image

## words

    ( - )

List all names in dictionary

## .s

    ( - )

Display all items on stack

## reset

    ( ...- )

Remove all items from stack

## depth

    ( -n )

Return number of items on stack

## formatted

    ( -a )

Variable; toggles whether **puts** uses escape sequences or not

## "

    ( ``-$ )

Parse and return a string

## __"

    ( ``-$ )

Prefix; parse and return a string

## tempString

    ( a-a )

Move a string to a temporary buffer

## rename:

    ( a``- )

Rename a function

## with|

    ( ``- )

Open a series of vocabularies, ending when ``  is encountered

## findInChain

    ( $a-df )

Open a chain (using **:with**) and search for a name. Closes the chain when done.

## global

    ( - )

Remove all vocabularies from the search order, leaving just the global dictionary 

## without

    ( - )

Remove a vocabulary from the search order 

## with

    ( ``- )

Add a vocabulary to the search order (parses for name)

## :with

    ( a- )

Add a vocabulary to the search order (by pointer)

## ;chain

    ( - )

End a vocabulary

## chain:

    ( ``- )

Create a new vocabulary

## .chain

    ( a- )

Class for vocabularies

## &lt;%&gt;

    ( a- )

Open a vocabulary. Use with caution

## %%

    ( - )

Close a vocabulary. Use with caution

## dicts

    ( -a )

Array; used by chained vocabularies and search order code

## __'

    ( $-n )

Return character following '

## __%

    ( $-n )

Prefix; treat number as binary (base 2)

## __#

    ( $-n )

Prefix; treat number as decimal (base 10)

## __$

    ( $-n )

Prefix; treat number as hexadecimal (base

## parsing

    ( - )

Set most recent function to **.parse** class

## .parse

    ( a- )

Class for parsing prefixes

## putn

    ( n- )

Display a number

## space

    ( - )

Display a space character (ASCII 32)

## clear

    ( - )

Clear the display

## toString

    ( n-$ )

Convert a number into a string

## binary

    ( - )

Switch **base** to  2

## octal

    ( - )

Switch **base** to  8

## hex

    ( - )

Switch **base** to 16

## decimal

    ( - )

Switch **base** to 10

## elements

    ( n``- )

Create a series of variables

## allot

    ( n- )

Allocate space in the heap

## string:

    ( $``- )

Create a string constant

## constant

    ( n``- )

Create a numeric constant

## variable

    ( ``- )

Create a new variable with an initial value of 0

## variable:

    ( n``- )

Create a new variable with an initial value 

## within

    ( xlu-f )

Is (x) within lower (l) and upper (u) bounds?

## if;

    ( f- )

Exit function if TOS is a non-zero flag

## ahead

    ( -a )

Used in conditionals; compiles a branch to be patched in later

## fill

    ( ann- )

Fill (n2) memory locations starting at (a) with value (n1)

## copy

    ( aan- )

Copy n values from source (a1) to dest (a2)

## each@

    ( ...t- )

Supercombinator for applying a quote to each item in various data structures.
Provide one of the following stack forms:

    ARRAY:  aq-
    BUFFER: anq-
    STRING: $q-
    LIST: lq-

For LIST, *l* should be a variable pointing to the li:doc

## &lt;each@&gt;

    ( ...t- )

Hook into **each@** for adding additional types

## iter

    ( nq- )

Run quote (n) times and push counter to stack each time. Counts up.

## iterd

    ( nq- )

Run quote (n) times and push counter to stack each time. Counts down.

## times

    ( nq- )

Run quote (n) times

## whend

    ( nqq-? )

Execute q1, with a copy of n on the stack.

If q1 returns a true flag, drop n, run q2 and exit the caller.

If not, discard q2 and return to the caller. 

## when

    ( nqq-n )

Execute q1, with a copy of n on the stack.

If q1 returns a true flag, run q2 and exit the caller.

If not, discard q2 and return to the  caller.

q2 is permitted to discard n, which will alter the stack effect.

## preserve

    ( aq- )

Given a variable (a) and a quote (q), preserve the contents of (a) while executing  the quote, and restore the original contents  of (a) after execution completes. (a) is removed from the stack before (q) is executed.

## cons

    ( ab-q )

Create a quote returning two data elements

## tri@

    ( xyzq- )

Apply q to x, y, and z

## tri*

    ( xyzqqq- )

Apply q1 to x, q2 to y, and q3 to z

## tri

    ( xqqq- )

Apply each quote to a copy of x

## bi@

    (  xyq- )

Apply q to x and y

## bi*

    ( xyqq- )

Apply q1 to x and q2 to y

## bi

    (  xqq- )

Apply each quote to a copy of x

## take

    ( qq-q )

5 [ . ]   =  [ [ . ] do 5 ]

## curry

    ( nq-q )

5 [ . ]   =  [ 5 [ . ] do ]

## until

    ( q- )

Execute quote until quote returns a flag of -1

## while

    ( q- )

Execute quote until quote returns a flag of

## []

    ( - )

Empty quote

## jump:

    ( ``- )

Compile a jump to another function

## `

    ( ``- )

Either execute a function, or compile the xt  and a call to the corresponding class         handler. This will also work with numbers

## compile-only

    ( ``- )

Set the most recent function to **.compiler** class

## immediate

    ( - )

Set the most recent function to **.macro** class

## .compiler

    ( a- )

Class for functions that can only be used inside a definition

## __2

    ( a- )

Prefix; execute function twice

## __-

    ( na- )

Prefix; execute function or data element and subtract value from value at address returned

## __+

    ( na- )

Prefix; execute function or data element and add value to value at address returned

## __!

    ( na- )

Prefix; execute function or data element and store value to address returned

## __@

    ( a-n )

Prefix; execute function or data element and fetch from addres returned

## __&

    ( a-a )

Prefix; returns address of a variable or function

## reclass:

    ( a``- )

Same as **reclass**, but parse for function to change class of

## reclass

    ( a- )

Change class of most recent function to specified class

## hide

    ( ``- )

Remove a name from the dictionary

## :hide

    ( a- )

Remove a name from a dictionary. Specify the address of a function. Used by **hide**

## xt-&gt;d

    ( a-d )

Given an address, return the corresponding dictionary header or 0 if not found

## d'

    ( ``-a )

Parse for a name and return the dictionary header corresponding to it

## HEADERS

    ( -n )

Returns number of private headers permitted

## default:

    ( ``- )

Compile call to default definition of a function, ignoring any revectoring

## is

    ( a``- )

Same as **:is**, but parses for name of function

## devector

    ( ``- )

Same as **:devector**, but parses for name of function

## }}

    ( - )

Close a namespace, sealing off private symbols

## ---reveal---

    ( - )

Switch to public portion of a namespace

## {{

    ( - )

Start a namespace (private portion)

## ?dup

    ( -n )

Duplicate TOS if non-zero. If zero, leave value alone

## --

    ( a- )

Decrement variable by 1

## ++

    ( a- )

Increment variable by 1

## -!

    ( na- )

Subtract value from value at address

## +!

    ( na- )

Add value to value at address

## tuck

    ( xy-yxy )

Put a copy of TOS under NOS

## rot

    ( xyz-yzx )

Rotate the top three values on the stack

## nip

    ( xy-y )

Drop the NOS from the stack

## :doc

    ( $- )

attach documentation string to latest defined function

## tabAsWhitespace

    ( -a )

Variable; treat tab as whitespace?

## build

    ( -$ )

String holding a build identifier

## version

    ( -$ )

String holding version information

## update

    ( -a )

Variable; flag indicating whether or not **redraw** should update the display

## base

    ( -a )

Variable; holds current base for numeric conversion and display

## eatLeading?

    ( -a )

Variable; indicates whether **accept** should ignore leading delimiters

## remapping

    ( -a )

Variable; indicates whether CR, LF, and TAB should be treated as whitespace

## which

    ( -a )

Variable; Holds pointer to most recently looked up header

## heap

    ( -a )

Variable; Pointer to current free location in heap

## ch

    ( -a )

Variable; Console height

## cw

    ( -a )

Variable; Console width

## memory

    ( -a )

Variable; Holds amount of memory provided by the VM

## fh

    ( -a )

Variable; Framebuffer height

## fw

    ( -a )

Variable; Framebuffer width

## fb

    ( -a )

Variable; Is canvas present?

## compiler

    ( -a )

Variable; holds compiler state

## last

    ( -a )

Variable; pointer to most recent dictionary  header

## listen

    ( - )

Top level interpreter. Reads and process input.

## ok

    ( - )

Displays the ``ok`` prompt

## notFound

    ( - )

Called when a name is not found. Calls **&lt;notFound&gt;** and displays an error message  if necessary

## &lt;notFound&gt;

    ( -f )

Called by **notFound**; hook for custom error handling. Used by the prefix system. Returns  a flag of 0 if the error is cleared, or -1 if not 

## '

    ( ``-a )

Interpret time: return the address ('xt') of a name

## find

    ( $-af )

Search for a name in the dictionary. Returns  a dictionary header and a flag

## boot

    ( - )

Called when the image first loads; use for custom startup routines

## isNumber?

    ( $-f )

See if a string is a valid number in the current **base**

## toNumber

    ( $-n )

Convert a string to a number

## numbers

    (  -a )

Function returning address of string containing all valid numeric characters

## atib

    (  -a )

Returns address of alternate text input buffer

## :is

    ( aa- )

Alter a function to point to a new function

## :devector

    ( a- )

Restore a function to its original state

## keepString

    ( a-a )

Move the string to a permanent location

## withLength

    ( a-an )

Same as **dup getLength**

## getLength

    ( a-n )

Return the length of a string

## compare

    ( $$-f )

Compare two strings for equality

## &gt;

    ( xy-f )

Compare two values for greater than

## &lt;

    ( xy-f )

Compare two values for less than

## &lt;=

    ( xy-f )

Compare for less than or equal to

## &gt;=

    ( xy-f )

Compare for greater than or equal to

## !=

    ( xy-f )

Compare two values for inequality.

## &lt;&gt;

    ( xy-f )

Compare two values for inequality. Use **!=** instead.

## ==

    ( xy-f )

Compare two values for equality.

## =

    ( xy-f )

Compare two values for equality. Use **==** instead

## sip

    ( nq-n )

Call a quote with an item on the stack, restoring that item after the quote returns

## dip

    ( nq-n )

Call a quote while temporarily hiding the top item on the stack

## ifFalse

    ( fq- )

Execute quote if flag is false

## ifTrue

    ( fq- )

Execute quote if flag is true

## if

    ( fqq- )

Execute first quote if flag is true, second  if false

## ]

    ( -a )

End a quote (code block)

## [

    ( - )

Start a quote (code block)

## (

    ( ``- )

Parse for ) and ignore everything it reads

## :

    ( ``- )

Calls **create**, changes class to **.word**, and turns **compiler** on.

## ]]

    ( - )

Turn compiler on

## [[

    ( - )

Turn compiler off

## create

    ( ``- )

Parse for a name and call **header**

## header

    ( $- )

Given a name, create a new header with a class of **.data**

## d-&gt;name

    ( a-a )

Given a dictionary header, return the address of the name. This is the actual start of the  name.

## d-&gt;doc

    ( a-a )

Given a dictionary header, return the address of a documentation string. Use **@** to get the actual pointer.

## d-&gt;xt

    ( a-a )

Given a dictionary header, return the address of the function start (*xt*). Use **@** to get the actual pointer.

## d-&gt;class

    ( a-a )

Given a dictionary header, return the address of the class handler. Use **@** to get the actual pointer.

## accept

    ( c- )

Read a string, ending with the specified character. The string is returned in **tib**

## getc

    ( -c )

Read a keypress and return the ASCII value on the stack

## remapKeys

    ( c-c )

Remap one ASCII value to another

## tib

    (  -a )

Returns address of text input buffer

## STRING-BUFFERS

    ( -n )

Return number of temporary string buffers

## STRING-LENGTH

    ( -n )

Return the max length for a string

## puts

    ( $- )

Display a string

## &lt;puts&gt;

    ( $- )

Helper; default way to display strings

## cr

    ( - )

Display a newline character

## putc

    ( c- )

Display a character

## redraw

    ( - )

Update the display. Can be disabled temporarily by **update**

## again

    ( R: -  C: a- )

Jump to the code following the most recent **repeat**

## repeat

    ( R: - C: -a )

Start an unconditional loop

## 0;

    ( n-n || n- )

If TOS is not zero, do nothing. If TOS is zero, drop TOS and exit the function

## pop

    ( -n )

Pop a value off the address stack

## push

    ( n- )

Push a value to the address stack

## ;

    ( - )

Compile an exit into a function and stop the compiler

## ;;

    ( - )

Compile an exit into a function, but do not stop compilation

## here

    ( -a )

Next free address in **heap**

## !+

    ( ca-a )

Store a value to an address, return next address

## @+

    ( a-ac )

Fetch a value from an address, return the next address and the value

## do

    ( a- )

Call a function by address

## negate

    ( x-y )

Invert sign of TOS

## mod

    ( xy-r )

Modulus of two numbers (xy)

## /

    ( xy-q )

Divide two numbers (x/y)

## off

    ( a- )

Set a variable to  0 (false)

## on

    ( a- )

Set a variable to -1 (true)

## not

    ( x-y )

Same as -1 xor; invert TOS and subtract 1

## over

    ( xy-xyx )

Place a copy of NOS over TOS

## wait

    ( - )

Wait for an I/O event

## in

    ( p-n )

Read a value from an I/O port

## out

    ( np- )

Write a value to an I/O port

## &gt;&gt;

    ( xy-n )

Shift bits right (x&gt;&gt;y)

## &lt;&lt;

    ( xy-n )

Shift bits left (x&lt;&lt;y)

## /mod

    ( xy-rq )

Divide and Remainder. This performs symmetric division

## *

    ( xy-n )

Multiply two values (x*y)

## -

    ( xy-n )

Subtract two values (x-y)

## +

    ( xy-n )

Add two values (x+y)

## !

    ( na- )

Store a value to a memory location

## @

    ( a-n )

Fetch a value from a memory location

## xor

    ( xy-n )

Bitwise XOR

## or

    ( xy-n )

Bitwise OR

## and

    ( xy-n )

Bitwise AND

## drop

    ( n- )

Drop TOS from the stack

## swap

    ( xy-yx )

Exchange positions of TOS and NOS

## 1-

    ( n-n )

Decrement TOS by 1 

## 1+

    ( n-n )

Increment TOS by 1 

## dup

    ( n-nn )

Duplicate TOS

## .primitive

    ( a- )

Class for functions corresponding to VM opcodes; used for simple optimizations

## .data

    ( a- )

Class for data (variables, literals, etc) 

## .macro

    ( a- )

Class for immediate functions

## .word

    ( a- )

Class for normal functions

## withClass

    ( ac- )

Execute a function via the specified class handler

## ,

    ( n- )

Place TOS **here** and increment **heap** by 1 
# vt'

## wrln
undocumented

## wr
undocumented

## vt:colors
undocumented

## |W
undocumented

## |w
undocumented

## |C
undocumented

## |c
undocumented

## |M
undocumented

## |m
undocumented

## |B
undocumented

## |b
undocumented

## |Y
undocumented

## |y
undocumented

## |G
undocumented

## |g
undocumented

## |R
undocumented

## |r
undocumented

## |K
undocumented

## |k
undocumented

## vt:bg
undocumented

## vt:fg
undocumented

## vt:hi
undocumented

## vt:lo
undocumented

## vt:curs
undocumented

## vt:cls
undocumented

## vt:emit
undocumented

## vt:bgc
undocumented

## vt:fgc
undocumented

## to:xterm
undocumented

## to:chgen
undocumented

## io:send
undocumented

## %%
undocumented
# dissect'

## endOfWord?
undocumented

## decompile
undocumented

## lookupOpcode
undocumented

## %%
undocumented
# types'

## LIST

    ( -n )

Type constant for linked lists

## STRING

    ( -n )

Type constant for strings

## BUFFER

    ( -n )

Type constant for buffers

## ARRAY

    ( -n )

Type constant for arrays

## %%
undocumented
# strings'

## splitAtChar:

    ( $``-$$ )

Parse for a character and call **splitAtChar**

## splitAtChar

    ( $c-$$ )

Search for a character and return two strings (up to and including (c), and after ($2))

## split

    ( $n-$$ )

Split a string into two parts

## reverse

    ( $-$ )

Reverse the characters in a string; returns a new string

## toUpper

    ( $-$ )

Convert a string to all upper case

## toLower

    ( $-$ )

Convert a string to all upper case

## appendChar

    ( $c-$ )

Append character to a string

## append

    ( $$-$ )

Append second string to first

## prepend

    ( $$-$ )

Append first string to second

## trimRight

    ( $-$ )

Trim whitespace from right side of string

## trimLeft

    ( $-$ )

Trim whitespace from left side of string

## getSubset

    ( $nn-$ )

Return a subset of ($) starting at (n1) with length of (n2)

## chop

    ( $-$ )

Return a new string, with the last byte removed

## findChar

    ( $c-a )

Search for a character within a string; return string starting at the character

## search

    ( $$-f )

Search for a string (2) within a string (1); return string starting with substring

## %%
undocumented
# buffer'

## set

    ( a- )

Set buffer to memory address and empty it

## size

    ( -n )

Number of values in buffer

## empty

    ( - )

Remove everything from the buffer

## get

    ( -c )

Read and remove value from buffer

## add

    ( c- )

Add value to end of buffer

## end

    ( -a )

Address at end of buffer

## start

    ( -a )

Get starting address of buffer

## %%
undocumented
# internals'

## string

    ( -a )

Helper function for strings

## quote

    ( -a )

Helper function for quotations

## %%
undocumented
# all
vt' dissect' types' strings' buffer' internals' 
