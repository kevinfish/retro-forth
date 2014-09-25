======
array'
======


--------
Overview
--------
This library provides functions for creating and working with simple
arrays.


--------------
Implementation
--------------
Arrays are stored as a linear sequence of values. In memory, they
are represented like:

::

  +---+--------------------+
  | 0 | number of elements |
  +---+--------------------+
  | 1 | first value        |
  +---+--------------------+
  | 2 | second value       |
  +---+--------------------+
  | n | ... nth value ...  |
  +---+--------------------+


--------
Examples
--------

::

  ( Create an array with four elements )
  ^array'new{ 1 2 3 4 } constant a

  ( Add 10 to each element in an array )
  a [ 10 + ] ^array'map

  ( Display an array )
  a ^array'display

  ( Average the values in an array )
  ^array'new{ 1 2 3 4 5 } [ ^array'sum ] [ ^array'length ] bi /

  ( Create an array of strings )
  [ "abc"  "def"  "ghi" ] ^array'fromQuote


---------
Functions
---------

+-------------+-----------+------------------------------------------------+
| Name        | Stack     | Usage                                          |
+=============+===========+================================================+
| new{        | "-a       | Create a new array. Parses and adds numbers to |
|             |           | the array until "}" is reached. Returns a      |
|             |           | pointer to the new array.                      |
+-------------+-----------+------------------------------------------------+
| display     | a-        | Display all values in the array                |
+-------------+-----------+------------------------------------------------+
| in?         | na-       | Returns -1 if value (n) is a value in the array|
+-------------+-----------+------------------------------------------------+
| stringIn?   | $n-       | Returns -1 if string ($) is a string in the    |
|             |           | array                                          |
+-------------+-----------+------------------------------------------------+
| map         | aq-       | Apply quote to each item in the array. Updates |
|             |           | the array with the values returned by the quote|
+-------------+-----------+------------------------------------------------+
| apply       | aq-       | Apply quote to each item in the array. Does not|
|             |           | modify the array contents.                     |
+-------------+-----------+------------------------------------------------+
| sum         | a-n       | Add all values in an array                     |
+-------------+-----------+------------------------------------------------+
| length      | a-n       | Get the number of items in an array            |
+-------------+-----------+------------------------------------------------+
| reverse     | a-        | Reverse the order of all elements in an array  |
+-------------+-----------+------------------------------------------------+
| append      | aa-a      | Append two arrays and return a new one         |
+-------------+-----------+------------------------------------------------+
| fromQuote   | q-a       | Generate an array from the values returned by a|
|             |           | quote                                          |
+-------------+-----------+------------------------------------------------+
| toQuote     | a-q       | Generate a quote from the values stored in an  |
|             |           | array                                          |
+-------------+-----------+------------------------------------------------+
| sort:bubble | a-        | Sort the items in an array using bubble sort   |
+-------------+-----------+------------------------------------------------+

==========
assertion'
==========


--------
Overview
--------
This vocabulary provides support for testing code in a clean,
predicatable manner.

Assertion predicates first check the stack for underflow; if
the stack is deep enough, their embedded predicates are applied;
if not, the assertion fails.

The result of each assertion - including the underflow check -
is ANDed with the assertionFlag which can then be tested after
the containing thread has finished executing; this is handled by
the .assertion word class.

For custom behaviour, revector preCond and/or postCond; by
default the pre-condition is an effective nop while the post-
condition simply prints 'Success' or 'Failure'.

Given that each assertion predicate mutates assertionFlag, use
of the word class .assertion is encouraged; this resets the
assertionFlag before execution and push its final value to the
stack before calling postCond when the thread exits.

NOTE: For simplicity of implementation, failure within a word of
class .assertion will not result in immediate termination;
instead, the false value of assertionFlag is left to propagate.


--------
Examples
--------
::

  ( Our assertions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ )

  ( n.b, underflow is dealt with sanely )
  : a1 ( xyz- ) 30 assert= 20 assert= 10 assert= ; assertion
  : a2 (  xy- ) assertTrue assertFalse ; assertion
  : a3 (   x- ) 5 assert< ; assertion

  ( Simple demo ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ )
  : go
    "\n----------" puts
    10 20 30 a1
    30 20 10 a1
    "\n----------" puts
    10 a1
    "\n----------" puts
    a1
    "\n----------" puts
    -1 0 a2
    0 -1 a2
    -1 -1 a2
    -1 a2
    "\n----------" puts
    3 a3
    4 a3
    5 a3
    a3
    "\n----------" puts
  ;

  clear go

  ( Adjusting pre- and post- conditions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ )
  : myPreCond .s ;
  : myPostCond [ "Fatal error: assertion failure." puts bye ] ifFalse ;
  &myPostCond is postCond
  &myPreCond is preCond

  ( 'go' will now exit on first failure )
  go


---------
Functions
---------

+---------------+-------+-----------------------------------------------------+
| Name          | Stack | Usage                                               |
+===============+=======+=====================================================+
| assertionFlag | -a    | Variable. This holds a true (-1) or false (0)       |
|               |       | value indicating whether the current set of         |
|               |       | assertions have passed or failed. The **.assertion**|
|               |       | class will set this to true automatically.          |
+---------------+-------+-----------------------------------------------------+
| assert        | f-    | Updates the **assertionFlag** using bitwise AND     |
+---------------+-------+-----------------------------------------------------+
| available     | n-f   | Checks to see if there are at least *n* items on the|
|               |       | stack. Returns true if there are, or false otherwise|
+---------------+-------+-----------------------------------------------------+
| assert=       | nn-   | Check to see if two values are equal                |
+---------------+-------+-----------------------------------------------------+
| assertFalse   | f-    | Check to see if flag is false (0)                   |
+---------------+-------+-----------------------------------------------------+
| assertTrue    | f-    | Check to see if flag is true (non-zero)             |
+---------------+-------+-----------------------------------------------------+
| assert>       | nn-   | Check to see if n1 is greather than n2              |
+---------------+-------+-----------------------------------------------------+
| assert>=      | nn-   | Check to see if n1 is greater than or equal to n2   |
+---------------+-------+-----------------------------------------------------+
| assert<       | nn-   | Check to see if n1 is less than n2                  |
+---------------+-------+-----------------------------------------------------+
| assert<=      | nn-   | Check to see if n1 is less than or equal to n2      |
+---------------+-------+-----------------------------------------------------+
| putAssertion  | f-    | Display 'Success' or 'Failure' based on flag        |
+---------------+-------+-----------------------------------------------------+
| preCond       | ``-`` | Hook, does nothing bu default                       |
+---------------+-------+-----------------------------------------------------+
| postCond      | f-    | Hook, displays 'Success' or 'Failure' by default    |
+---------------+-------+-----------------------------------------------------+
| .assertion    | a-    | Class handler for assertions                        |
+---------------+-------+-----------------------------------------------------+
| assertion     | ``-`` | Change the class of a function to **.asssertion**   |
+---------------+-------+-----------------------------------------------------+

====
bad'
====


--------
Overview
--------
The Ngaro virtual machine underlying Retro does not provide access to memory
in units other than a standard cell. This simplifies things in many ways, but
can be very wasteful of memory, especially when dealing with large amounts of
text. This library provides support for dealing with byte-level access to the
Ngaro memory.


-----------
Terminology
-----------
*byte*: A unit of memory consisting of eight bits.

*cell*: A unit of memory consisting of thirty two bits.


-------
Loading
-------
::

  needs bad'


-------
Example
-------
::

  1024 ^bad'newPool
  'e 0 ^bad'b!
  'f 1 ^bad'b!
  'g 2 ^bad'b!
  'h 3 ^bad'b!
  ^bad'pool @ putn
  2 ^bad'b@ putn


---------
Functions
---------
+----------+--------+------------------------------------------------+
| Function | Stack  | Used For                                       |
+==========+========+================================================+
| pool     | -a     | Variable. This holds a pointer to the current  |
|          |        | byte addressable memory pool                   |
+----------+--------+------------------------------------------------+
| unpack   | c-bbbb | Given a byte-packed cell on the stack return   |
|          |        | the bytes it contains                          |
+----------+--------+------------------------------------------------+
| pack     | bbbb-c | Pack four byes into a cell, return the cell on |
|          |        | the stack                                      |
+----------+--------+------------------------------------------------+
| b@       | a-b    | Fetch a byte from the active byte addressable  |
|          |        | memory pool                                    |
+----------+--------+------------------------------------------------+
| b!       | ba-    | Store a byte into the currently active byte    |
|          |        | addressable memory pool                        |
+----------+--------+------------------------------------------------+
| b@+      | a-ab   | Fetch a byte from the active byte addressable  |
|          |        | memory pool. Returns next byte address and the |
|          |        | byte fetched                                   |
+----------+--------+------------------------------------------------+
| b!+      | ba-a   | Store a byte into the currently active byte    |
|          |        | addressable memory pool. Returns next byte     |
|          |        | address                                        |
+----------+--------+------------------------------------------------+
| newPool  |  n-    | Allocate a new pool of *n* characters and set  |
|          |        | **pool** to point to it                        |
+----------+--------+------------------------------------------------+

=========
bstrings'
=========


--------
Overview
--------
This library builds on the *byte addressing* vocabulary (**bad'**) to provide
support for creating and working with byte-packed strings.

Generally it's better to use the cell-based strings. They are larger in memory,
but much faster to work with. If space is critical though, this library can be
used to significantly reduce the memory consumed, at the cost of performance.

As an example of how this works, consider a simple string: "This is a test."

::

  "This is a test."

Retro will create a string in memory that looks like:

::

  0000 T
  0001 h
  0002 i
  0003 s
  0004 <space>
  0005 i
  0006 s
  0007 <space>
  0008 a
  0009 <space>
  0010 t
  0011 e
  0012 s
  0013 t
  0014 .
  0015 <terminator>

So 16 cells used. If we byte pack this, it reduces considerably:

::

  0000 This
  0001  is
  0002 a te
  0003 st<terminator>

Down to four cells. It's a straightforward 4:1 reduction.


-----------
Terminology
-----------
*byte*: A unit of memory consisting of eight bits.

*cell*: A unit of memory consisting of thirty two bits.


-------
Loading
-------
::

  needs bstrings'


---------
Functions
---------
+------------+--------+------------------------------------------------+
| Function   | Stack  | Used For                                       |
+============+========+================================================+
| pack       | $-a    | Convert a standard string into a byte packed   |
|            |        | string                                         |
+------------+--------+------------------------------------------------+
| getLength  | a-n    | Return the length of a byte packed string (in  |
|            |        | characters)                                    |
+------------+--------+------------------------------------------------+
| unpack     | a-$    | Convert a byte packed string into a standard   |
|            |        | string                                         |
+------------+--------+------------------------------------------------+
| withLength | a-an   | Return the length (in characters) and address  |
|            |        | of a byte-packed string                        |
+------------+--------+------------------------------------------------+
| puts       | a-     | Display a bye packed string                    |
+------------+--------+------------------------------------------------+
| toLower    | a-a    | Convert a byte packed string to lowercase      |
+------------+--------+------------------------------------------------+
| toUpper    | a-a    | Convert a byte packed string to uppercase      |
+------------+--------+------------------------------------------------+

=========
calendar'
=========


--------
Overview
--------
This vocabulary provides support for doing calculations involving time
and dates.


---------
Functions
---------
+-----------------+-------+--------------------------------------+
| Function        | Stack | Used For                             |
+=================+=======+======================================+
| MINUTES/WEEK    | -n    | Number of minutes in a week          |
+-----------------+-------+--------------------------------------+
| MINUTES/DAY     | -n    | Number of minutes in a day           |
+-----------------+-------+--------------------------------------+
| MINUTES/HOUR    | -n    | Number of minutes in an hour         |
+-----------------+-------+--------------------------------------+
| HOURS/DAY       | -n    | Number of hours in a day             |
+-----------------+-------+--------------------------------------+
| DAYS/WEEK       | -n    | Number of days in a week             |
+-----------------+-------+--------------------------------------+
| DAYS/YEAR       | -n    | Number of days in a typical year     |
+-----------------+-------+--------------------------------------+
| WEEKS/YEAR      | -n    | Number of weeks in a year            |
+-----------------+-------+--------------------------------------+
| SECONDS/MINUTE  | -n    | Number of seconds in a minute        |
+-----------------+-------+--------------------------------------+
| SECONDS/HOUR    | -n    | Number of seconds in an hour         |
+-----------------+-------+--------------------------------------+
| SECONDS/DAY     | -n    | Number of seconds in a day           |
+-----------------+-------+--------------------------------------+
| dayToName       | n-$   | Convert the number of a day to a     |
|                 |       | three letter abbreviation            |
+-----------------+-------+--------------------------------------+
| daysPerMonth    | m-n   | Return the number of days in a given |
|                 |       | month                                |
+-----------------+-------+--------------------------------------+
| secondsPerMonth | m-n   | Return the number of seconds in a    |
|                 |       | given month                          |
+-----------------+-------+--------------------------------------+
| isLeapYear?     | y-f   | Return true (-1) if the year is a    |
|                 |       | leap year or false (0) otherwise     |
+-----------------+-------+--------------------------------------+
| secondsPerYear  | y-n   | Return the number of seconds in a    |
|                 |       | given year                           |
+-----------------+-------+--------------------------------------+
| toJulian        | dmy-j | Convert a date into a Julian day     |
+-----------------+-------+--------------------------------------+
| toDayOfWeek     | dmy-n | Convert a date to the numeric day of |
|                 |       | the week                             |
+-----------------+-------+--------------------------------------+


=======
canvas'
=======
This library provides support for the canvas device provided by the
JavaScript implementation of Ngaro.


-------
Loading
-------
Since the JavaScript implementation has no file I/O, build an image with
this loaded manually.

::

  needs infix'
  include vm/web/dumpImage.rx


--------
Examples
--------

::

  100 100 50 ^canvas'red ^canvas'solid ^canvas'circle


---------
Functions
---------

+---------------+-----------+-------------------------------------------------+
| Name          | Stack     | Usage                                           |
+===============+===========+=================================================+
| mouse         |     -xy   |  Current mouse coordinates                      |
+---------------+-----------+-------------------------------------------------+
| click?        |     -f    |  Is mouse button clicked?                       |
+---------------+-----------+-------------------------------------------------+
| setColor      |    c-     |  Set color to *c*. Used by various other words  |
+---------------+-----------+-------------------------------------------------+
| pixel         |   xy-     |  Draw a pixel in the current color              |
+---------------+-----------+-------------------------------------------------+
| solid         |   ``-``   |  Modifier; tells **box** and **circle** to draw |
|               |           |  a filled in shape rather than an outline       |
+---------------+-----------+-------------------------------------------------+
| box           | xyhw-     |  Draw a box                                     |
+---------------+-----------+-------------------------------------------------+
| vline         |  xyh-     |  Draw a vertical line                           |
+---------------+-----------+-------------------------------------------------+
| hline         |  xyw-     |  Draw a horizontal line                         |
+---------------+-----------+-------------------------------------------------+
| circle        |  xyw-     |  Draw a circle                                  |
+---------------+-----------+-------------------------------------------------+
| black         |   ``-``   |  Set color                                      |
+---------------+-----------+-------------------------------------------------+
| blue          |   ``-``   |  Set color                                      |
+---------------+-----------+-------------------------------------------------+
| green         |   ``-``   |  Set color                                      |
+---------------+-----------+-------------------------------------------------+
| cyan          |   ``-``   |  Set color                                      |
+---------------+-----------+-------------------------------------------------+
| red           |   ``-``   |  Set color                                      |
+---------------+-----------+-------------------------------------------------+
| purple        |   ``-``   |  Set color                                      |
+---------------+-----------+-------------------------------------------------+
| brown         |   ``-``   |  Set color                                      |
+---------------+-----------+-------------------------------------------------+
| gray          |   ``-``   |  Set color                                      |
+---------------+-----------+-------------------------------------------------+
| darkGray      |   ``-``   |  Set color                                      |
+---------------+-----------+-------------------------------------------------+
| brightBlue    |   ``-``   |  Set color                                      |
+---------------+-----------+-------------------------------------------------+
| brightGreen   |   ``-``   |  Set color                                      |
+---------------+-----------+-------------------------------------------------+
| brightCyan    |   ``-``   |  Set color                                      |
+---------------+-----------+-------------------------------------------------+
| brightRed     |   ``-``   |  Set color                                      |
+---------------+-----------+-------------------------------------------------+
| magenta       |   ``-``   |  Set color                                      |
+---------------+-----------+-------------------------------------------------+
| yellow        |   ``-``   |  Set color                                      |
+---------------+-----------+-------------------------------------------------+
| white         |   ``-``   |  Set color                                      |
+---------------+-----------+-------------------------------------------------+
| dimensions    |     -hw   |  Return height and width of canvas              |
+---------------+-----------+-------------------------------------------------+

=======
casket'
=======

--------
Overview
--------
Casket is a lightweight framework intended to simplify the development of
web apps in Retro.

Casket grew out of my work on the Corpse blog and later through work on the
Rancid IRC log viewer.


---------
Functions
---------
+------------------+-------+--------------------------------------------------+
| Name             | Stack | Usage                                            |
+==================+=======+==================================================+
| serve:           | """-  | Serve a file with a specific mime type           |
+------------------+-------+--------------------------------------------------+
| decode           | $-    | Decode a URL encoded string                      |
+------------------+-------+--------------------------------------------------+
| dispatch         | ``-`` | Look for a view handler (e.g., /index) and call  |
|                  |       | it, or call **/404** if none is found            |
+------------------+-------+--------------------------------------------------+
| doBeforeDispatch | ``-`` | Code to execute before processing paths. This is |
|                  |       | always called before **dispatch**.               |
+------------------+-------+--------------------------------------------------+
| /                | ``-`` | Default index page                               |
+------------------+-------+--------------------------------------------------+
| /404             | ``-`` | Default 404 error page                           |
+------------------+-------+--------------------------------------------------+
| withTemplate     | $-    | Include and render a template file using the     |
|                  |       | **tputs** function                               |
+------------------+-------+--------------------------------------------------+
| withFile         | $-    | Include and render a raw file                    |
+------------------+-------+--------------------------------------------------+
| tputs            | ...$- | This is a replacement for **puts**, which adds   |
|                  |       | additional escape sequences for use with the     |
|                  |       | templates.                                       |
+------------------+-------+--------------------------------------------------+
| eputs            | $-    | This is a replacement for **puts**, which escapes|
|                  |       | html entities for use with <pre> and such        |
+------------------+-------+--------------------------------------------------+
| Content-type:    | "-    | Generate a MIME header for a file or view        |
+------------------+-------+--------------------------------------------------+
| getFormData      | -$    | Process a form and return the elements as a text |
|                  |       | string                                           |
+------------------+-------+--------------------------------------------------+
| getRequest       | ``-`` | Internal: Read the PATH_INFO environment variable|
|                  |       | and parse for **casket:path** and                |
|                  |       | **casket:options**                               |
+------------------+-------+--------------------------------------------------+
| casket:url       | -a    | Function returning application url               |
+------------------+-------+--------------------------------------------------+
| casket:root      | -a    | Function returning path to application root      |
+------------------+-------+--------------------------------------------------+
| casket:form      | -a    | Internal buffer for form data                    |
+------------------+-------+--------------------------------------------------+
| casket:buffer    | -a    | Internal buffer                                  |
+------------------+-------+--------------------------------------------------+
| casket:options   | -a    | Returns optional data following view)            |
+------------------+-------+--------------------------------------------------+
| casket:path      | -a    | Returns requested view (e.g., /index)            |
+------------------+-------+--------------------------------------------------+


-------
Example
-------

application.rx
==============
::

  needs casket'
  with casket'

 : /index
    Content-type: text/html
    "index.erx" withTemplate ;

  : /test
    Content-type: text/html
    casket:options "test.erx" withTemplate ;

  [ ( -$ ) "/path/to/app/" ] is casket:root
  [ ( -$ ) "http://domain.ext/path/to/cgi" ] is casket:url
  &/index is /
  &dispatch is boot
  save bye


index.erx
=========
::

  <html>
  <body><h1>Test of Casket</h1>
  <p><a href='%u/test/apple'>test apple</a>
     or <a href='%u/test/banana'>test banana</a>.</p>
  </body></html>


test.erx
========
::

  <html>"  tputs
  <body><h1>You requested...</h1>
  <p>An %s.</p>
  </body></html>


=====
char'
=====
This library provides a vocabulary for simple operations on ASCII characters.


-------
Loading
-------
The following should suffice:

::

  needs char'


--------
Examples
--------

::

  97 ^char'isChar?
  'a dup ^char'isUpper? [ ^char'toLower ] ifTrue


---------
Functions
---------
+---------------+---------+------------------------------------------------+
| Name          | Stack   | Usage                                          |
+===============+=========+================================================+
| isChar?       | c-f     | Return true if a given value is an alphabetic  |
|               |         | character (A-Z or a-z). If not, return false   |
+---------------+---------+------------------------------------------------+
| isUpper?      | c-f     | Return true if character is uppercase, false   |
|               |         | otherwise                                      |
+---------------+---------+------------------------------------------------+
| isLower?      | c-f     | Return true if character is lowercase, false   |
|               |         | otherwise                                      |
+---------------+---------+------------------------------------------------+
| isNumeric?    | c-f     | Return true if character is between 0 - 9      |
|               |         | inclusive, or false otherwise                  |
+---------------+---------+------------------------------------------------+
| isWhitespace? | c-f     | Return true if character is a space, tab, or   |
|               |         | end of line. Returns false otherwise           |
+---------------+---------+------------------------------------------------+
| isVisible?    | c-f     | Return true if character is visible, or false  |
|               |         | if it is a control-type character              |
+---------------+---------+------------------------------------------------+
| toUpper       | c-c     | Convert a lowercase character to uppercase.    |
|               |         | This will only work on a lowercase character.  |
+---------------+---------+------------------------------------------------+
| toLower       | c-c     | Convert an upperacase character to lowercase.  |
|               |         | This will only work on an uppercase character. |
+---------------+---------+------------------------------------------------+
| toString      | c-$     | Convert a character into a string              |
+---------------+---------+------------------------------------------------+

============
combinators'
============


--------
Overview
--------
This library serves as a home to various combinators that are useful, but not
essential to the core Retro language.


-------
Loading
-------
::

  needs combinators'


---------
Functions
---------
+----------+---------+------------------------------------------------+
| Function | Stack   | Used For                                       |
+==========+=========+================================================+
| loopd    | seq-    | Execute a loop. Index starts at high value (e) |
|          |         | and decrements to low value (s). The loop body |
|          |         | is quote (q).                                  |
+----------+---------+------------------------------------------------+
| indexd   | -n      | Return the current index for a decrementing    |
|          |         | loop.                                          |
+----------+---------+------------------------------------------------+
| loopi    | seq-    | Execute a loop. Index starts at low value (s)  |
|          |         | and increments to high value (e). The loop body|
|          |         | is quote (q).                                  |
+----------+---------+------------------------------------------------+
| indexi   | -n      | Return the current index for an incrementing   |
|          |         | loop.                                          |
+----------+---------+------------------------------------------------+

========
console'
========

.. class:: corefunc

+-----------------+-----------+-----------------------------------------------+
| Function        | Stack     | Notes                                         |
+=================+===========+===============================================+
|   colors        |     -a    |  Variable; indicates if console exists        |
+-----------------+-----------+-----------------------------------------------+
|   esc           |    $-     |  Display a string starting as an escape       |
|                 |           |  sequence                                     |
+-----------------+-----------+-----------------------------------------------+
|   at-xy         |   xy-     |  Move cursor to x,y                           |
+-----------------+-----------+-----------------------------------------------+
|   home          |   ``-``   |  Move cursor to top left corner               |
+-----------------+-----------+-----------------------------------------------+
|   black         |   ``-``   |  Set foreground color                         |
+-----------------+-----------+-----------------------------------------------+
|   onBlack       |   ``-``   |  Set background color                         |
+-----------------+-----------+-----------------------------------------------+
|   red           |   ``-``   |  Set foreground color                         |
+-----------------+-----------+-----------------------------------------------+
|   onRed         |   ``-``   |  Set background color                         |
+-----------------+-----------+-----------------------------------------------+
|   green         |   ``-``   |  Set foreground color                         |
+-----------------+-----------+-----------------------------------------------+
|   onGreen       |   ``-``   |  Set background color                         |
+-----------------+-----------+-----------------------------------------------+
|   yellow        |   ``-``   |  Set foreground color                         |
+-----------------+-----------+-----------------------------------------------+
|   onYellow      |   ``-``   |  Set background color                         |
+-----------------+-----------+-----------------------------------------------+
|   blue          |   ``-``   |  Set foreground color                         |
+-----------------+-----------+-----------------------------------------------+
|   onBlue        |   ``-``   |  Set background color                         |
+-----------------+-----------+-----------------------------------------------+
|   magenta       |   ``-``   |  Set foreground color                         |
+-----------------+-----------+-----------------------------------------------+
|   onMagenta     |   ``-``   |  Set background color                         |
+-----------------+-----------+-----------------------------------------------+
|   cyan          |   ``-``   |  Set foreground color                         |
+-----------------+-----------+-----------------------------------------------+
|   onCyan        |   ``-``   |  Set background color                         |
+-----------------+-----------+-----------------------------------------------+
|   white         |   ``-``   |  Set foreground color                         |
+-----------------+-----------+-----------------------------------------------+
|   onWhite       |   ``-``   |  Set background color                         |
+-----------------+-----------+-----------------------------------------------+
|   normal        |   ``-``   |  Reset colors to default                      |
+-----------------+-----------+-----------------------------------------------+
|   bold          |   ``-``   |  Set color attribute to bold/bright           |
+-----------------+-----------+-----------------------------------------------+
|   dimensions    |     -hw   |  Return height and width of console           |
+-----------------+-----------+-----------------------------------------------+

=======
crypto'
=======

--------
Overview
--------
I enjoy dabbling with logic puzzles and classical forms of encryption.
This vocabulary is intended to be a tool to help with these diversions.


---------
Functions
---------

+-----------------+-------+----------------------------------------------------+
| Name            | Stack | Usage                                              |
+=================+=======+====================================================+
| ceaser          | $n-$  | Encrypt (or decrypt) a string using the ceaser     |
|                 |       | algorithm. The value n is the number of            |
|                 |       | characters to shift the alphabet by.               |
+-----------------+-------+----------------------------------------------------+
| rot13           | $-$   | Encrypt or decrypt a string using rot13. This      |
|                 |       | is a trivial variant of the ceaser algorithm.      |
+-----------------+-------+----------------------------------------------------+
| tapcode:encrypt | $-a   | Encrypt a string into a tapcode array              |
+-----------------+-------+----------------------------------------------------+
| tapcode:decrypt | a-$   | Decrypt a tapcode array into a string              |
+-----------------+-------+----------------------------------------------------+
| pigpen:encrypt  | $-a   | Convert a string into a pigpen encoded array.      |
|                 |       | To display the pigpen sequence, try:               |
|                 |       |                                                    |
|                 |       | ::                                                 |
|                 |       |                                                    |
|                 |       |   "ab" ^crypto'toPigpen [ puts space ] ^array'apply|
+-----------------+-------+----------------------------------------------------+
| pigpen:decrypt  | a-$   | Decode a pigpen encoded array into a string        |
+-----------------+-------+----------------------------------------------------+
| keyword:setKey  | $-    | Set the key to use for keyword encryption. This    |
|                 |       | should be 26 characters long.                      |
+-----------------+-------+----------------------------------------------------+
| keyword:encrypt | $-$   | Encrypt a string using keyword encryption          |
+-----------------+-------+----------------------------------------------------+
| keyword:decrypt | $-$   | Decrypt a string using keyword encryption          |
+-----------------+-------+----------------------------------------------------+

========
decimal'
========


--------
Overview
--------

This vocabulary provides a prefix and some functions for working with
simple decimal values. It's not intended as a replacement for fixed or
floating point math.


---------
Functions
---------

+----------+-------+-------------------------------------------------------+
| Function | Stack | Usage                                                 |
+==========+=======+=======================================================+
| __d      | "-n   | Prefix to parse a value like "nn.nnn" into an integer |
+----------+-------+-------------------------------------------------------+
| putn     | n-    | Display a decimal number, based on **scale**          |
+----------+-------+-------------------------------------------------------+
| scale    | -a    | Variable containing the number of decimal places      |
+----------+-------+-------------------------------------------------------+
| split    | -n    | Return a factor to divide by for display/working with |
|          |       | decimal values                                        |
+----------+-------+-------------------------------------------------------+

==========
decompose'
==========


--------
Overview
--------
The core Casket framework provides minimal templating, but no easy way to
generate HTML from code.


This vocabulary provides support for generating HTML using a series of
combinators. Textual data returned will be displayed (using **^casket'tputs**),
and various attributes are supported.


-------
Loading
-------
::

  needs decompose'


-------
Example
-------
::

  : /index
    Content-type: text/html
    [ [ [ "Hello" ] title ] head
      [ [ "Welcome" ] h1
        [ "This is a paragraph." ] p ] body ] html ;


==========
decorator'
==========


--------
Overview
--------
A decorator extends an existing function by appending ocde that will be
executed before (and possibly after) execution of the original function.


-----
Usage
-----
::

  ok  : foo + putn cr ;
  ok  1
  ok  2
  ok  foo 3
  ok  : demo 2over "%d + %d = " puts ;
  ok  &foo
  ok  &demo
  ok  decorate
  ok  1
  ok  2
  ok  foo 2 + 1 = 3
  ok


---------
Functions
---------
+------------+-------+----------------------------------------+
| Name       | Stack | Usage                                  |
+============+=======+========================================+
| decorate   | aa-   | Apply decoration (a2) to function (a1) |
+------------+-------+----------------------------------------+
| undecorate | a-    | Remove a decoration from a function    |
+------------+-------+----------------------------------------+

=====
diet'
=====


--------
Overview
--------
This library reduces overall memory consumption by reducing the string length
and number of string buffers.

While easy enough to do manually, this automates the process and makes it
easily reversible.


-----
Usage
-----
After loading, add a call to the function desired to your application code.
E.g.,

::

  [ ^diet'extreme ] is boot



---------
Functions
---------

+-------------+-----------+------------------------------------------------+
| Name        | Stack     | Usage                                          |
+=============+===========+================================================+
| shrink      | ``-``     | Reduce string length. This is invoked during   |
|             |           | the initial load of the library                |
+-------------+-----------+------------------------------------------------+
| extreme     | ``-``     | Further reductions of string length and reduces|
|             |           | the buffers                                    |
+-------------+-----------+------------------------------------------------+
| max         | ``-``     | Shrink as far as possible. Disables hidden     |
|             |           | headers and temporary strings                  |
+-------------+-----------+------------------------------------------------+
| bloat       | ``-``     | Restore buffers and string length to the       |
|             |           | default                                        |
+-------------+-----------+------------------------------------------------+

========
dissect'
========


--------
Overview
--------
This vocabulary provides a basic decompiler for the Ngaro instruction set.


-----
Usage
-----


---------
Functions
---------
+--------------+-------+-------------------------------------------------+
| Name         | Stack | Usage                                           |
+==============+=======+=================================================+
| lookupOpcode | n-$ff |                                                 |
+--------------+-------+-------------------------------------------------+
| decompile    | a-a$  |                                                 |
+--------------+-------+-------------------------------------------------+
| endOfWord?   | a-f   |                                                 |
+--------------+-------+-------------------------------------------------+

=====
enum'
=====

--------
Overview
--------
This vocabulary adds support for enumerated values.


-----
Usage
-----
::

  0 enum A enum B enum C enum D drop

Or

::

  0 enum| A B C D |


---------
Functions
---------
+-----------+-------+---------------------------------------------------------+
| Function  | Stack | Usage                                                   |
+===========+=======+=========================================================+
| step      | n-n   | Increment for next enumerated value. Defaults to **1+** |
|           |       | but can be revectored as needed                         |
+-----------+-------+---------------------------------------------------------+
| enum      | n"-n  | Create an enumerated constant, and increment the value  |
|           |       | using **step**                                          |
+-----------+-------+---------------------------------------------------------+
| ``enum|`` | n"-   | Create a series of enumerated values, incremented by    |
|           |       | **step**                                                |
+-----------+-------+---------------------------------------------------------+

=====
eval'
=====


--------
Overview
--------
This library provides a vocabulary for evaluating code contained in strings.

Retro was not designed to allow textual strings to be evaluated. This works
by remapping the keyboard input to read from a string, until the string is
exhausted. Additionally, it has a built-in varient of the listener to process
input immediately, rather than after the function returns.

This library should be used only if there's no other (clean) way to solve a
problem.


-------
Loading
-------
The following should suffice:

::

  needs eval'


--------
Examples
--------

::

  : foo  ( -n )  99 "54 + putn" ^eval'eval 5 ;

  ^eval'ifNotDefined foo { 100 constant foo }
  ^eval'ifDefined foo { foo 10 * putn }


---------
Functions
---------

+---------------+-------+------------------------------------------------------------------+
| Function      | Stack | Description                                                      |
+===============+=======+==================================================================+
| eval          | $-    | Evaluate a string                                                |
+---------------+-------+------------------------------------------------------------------+
| ifDefined     | ""-   | Parse for a name, if found, execute code in the following string |
|               |       | block. Otherwise ignore the string block.                        |
+---------------+-------+------------------------------------------------------------------+
| ifNotDefined  | ""-   | Parse for a name, if not found, execute code in the following    |
|               |       | string block. If found, ignore the string block.                 |
+---------------+-------+------------------------------------------------------------------+

========
fiction'
========

--------
Overview
--------

I have always enjoyed playing interactive fiction games. This library provides a
framework for building simple games of this type. It's been used for an implemention
of *Cloak of Darkness*, as well as various small puzzle games for my children.


---------
Functions
---------

+-------------------+-----------+------------------------------------------------+
| Name              | Stack     | Usage                                          |
+===================+===========+================================================+
| ROOM              | -n        | Item type for rooms                            |
+-------------------+-----------+------------------------------------------------+
| ITEM              | -n        | Item type for items                            |
+-------------------+-----------+------------------------------------------------+
| .description      | a-a       | Object field. Describe the item                |
+-------------------+-----------+------------------------------------------------+
| .shortDescription | a-a       | Object field. Short name for object            |
+-------------------+-----------+------------------------------------------------+
| .type             | a-a       | Object field. Type. ROOM or ITEM               |
+-------------------+-----------+------------------------------------------------+
| .onLook           | a-a       | Object field. Custom handler for "look"        |
+-------------------+-----------+------------------------------------------------+
| .onEntry          | a-a       | Object field. Custom handler for entry event   |
+-------------------+-----------+------------------------------------------------+
| .location         | a-a       | Object field. Where is this object?            |
+-------------------+-----------+------------------------------------------------+
| .visited          | a-a       | Object field. Have we been here before?        |
+-------------------+-----------+------------------------------------------------+
| .toNorth          | a-a       | Object field. Pointer to location to the north |
+-------------------+-----------+------------------------------------------------+
| .toSouth          | a-a       | Object field. Pointer to location to the south |
+-------------------+-----------+------------------------------------------------+
| .toEast           | a-a       | Object field. Pointer to location to the east  |
+-------------------+-----------+------------------------------------------------+
| .toWest           | a-a       | Object field. Pointer to location to the west  |
+-------------------+-----------+------------------------------------------------+
| .onNorth          | a-a       | Object field. Custom handler for "north"       |
+-------------------+-----------+------------------------------------------------+
| .onSouth          | a-a       | Object field. Custom handler for "south"       |
+-------------------+-----------+------------------------------------------------+
| .onEast           | a-a       | Object field. Custom handler for "east"        |
+-------------------+-----------+------------------------------------------------+
| .onWest           | a-a       | Object field. Custom handler for "west"        |
+-------------------+-----------+------------------------------------------------+
| .postDescription  | a-a       | Object field. Custom handler run after showing |
|                   |           |               a description                    |
+-------------------+-----------+------------------------------------------------+
| .preDescription   | a-a       | Object field. Custom handler run before showing|
|                   |           |               a desription                     |
+-------------------+-----------+------------------------------------------------+
| .onRead           | a-a       | Object field. custom handler for "read"        |
+-------------------+-----------+------------------------------------------------+
| describes         | $"-       | Add a description to an object                 |
+-------------------+-----------+------------------------------------------------+
| nameOf            | $"-       | Specify a short name for an object             |
+-------------------+-----------+------------------------------------------------+
| room              | "-        | Create a new ROOM object                       |
+-------------------+-----------+------------------------------------------------+
| invertDirection   | a$-a      |                                                |
+-------------------+-----------+------------------------------------------------+
| is                | a""""-    |                                                |
+-------------------+-----------+------------------------------------------------+
| items             | -L        | Linked list of all known items                 |
+-------------------+-----------+------------------------------------------------+
| item              | "-        | Create a new ITEM object                       |
+-------------------+-----------+------------------------------------------------+
| contains          | a"-       | Mark that a room contains an item              |
+-------------------+-----------+------------------------------------------------+
| has?              | a-af      | Does the player have an item?                  |
+-------------------+-----------+------------------------------------------------+
| displayItem       | a-        | Display the name of an item                    |
+-------------------+-----------+------------------------------------------------+
| inventory         | ``-``     | Display all items the player has               |
+-------------------+-----------+------------------------------------------------+


======
files'
======

.. class:: corefunc

+-----------------+-----------+-----------------------------------------------+
| Function        | Stack     | Notes                                         |
+=================+===========+===============================================+
|   :R            |     -n    |  Mode for reading a file. Does not create file|
+-----------------+-----------+-----------------------------------------------+
|   :W            |     -n    |  Mode for writing a file                      |
+-----------------+-----------+-----------------------------------------------+
|   :A            |     -n    |  Mode for appending to a file                 |
+-----------------+-----------+-----------------------------------------------+
|   :M            |     -n    |  Mode for modifying a file. Does not create   |
|                 |           |  file.                                        |
+-----------------+-----------+-----------------------------------------------+
|   open          |   $m-h    |  Open a file. Will return a handle. Valid     |
|                 |           |  handles will be non-zero. A zero handle      |
|                 |           |  indicates failure to open a file.            |
+-----------------+-----------+-----------------------------------------------+
|   read          |    h-c    |  Read a byte from a file. This returns the    |
|                 |           |  byte.                                        |
+-----------------+-----------+-----------------------------------------------+
|   write         |   ch-f    |  Write a byte to a file. This returns a flag  |
|                 |           |  indicating the number of bytes written.      |
|                 |           |  (Should always equal '1')                    |
+-----------------+-----------+-----------------------------------------------+
|   close         |    h-f    |  Close an open file. Returns a flag of zero if|
|                 |           |  unable to close, or non-zero if successful.  |
+-----------------+-----------+-----------------------------------------------+
|   pos           |    h-n    |  Get current position in a file               |
+-----------------+-----------+-----------------------------------------------+
|   seek          |   nh-f    |  Seek a position in a file                    |
+-----------------+-----------+-----------------------------------------------+
|   size          |    h-n    |  Return size of open file                     |
+-----------------+-----------+-----------------------------------------------+
|   delete        |    $-f    |  Delete a file. Returns a handle. Non-zero if |
|                 |           |  successful, zero if failed.                  |
+-----------------+-----------+-----------------------------------------------+
|   slurp         |   a$-n    |  Read a file into a buffer                    |
+-----------------+-----------+-----------------------------------------------+
|   spew          |  an$-n    |  Write (n) bytes from address (a) into a file |
+-----------------+-----------+-----------------------------------------------+
|   readLine      |    h-$    |  Read a line from a file                      |
+-----------------+-----------+-----------------------------------------------+
|   writeLine     |   $h-     |  Write a string to a file                     |
+-----------------+-----------+-----------------------------------------------+

======
forth'
======

--------
Overview
--------
This vocabulary will make Retro closer to a traditional Forth system. When
it is visible, some things will be significantly different than in a standard
Retro system, but it will also be easier to port existing Forth code.

Eventually this should provide a large subset of the ANS Forth standard, with
the limitation that only lowercase function names are provided.


-----
Notes
-----


Strings
=======
Strings in ANS Forth are represented either as counted, or address and length
pairs.

This differs from the strings in Retro which are represented as null-terminated
character arrays. So existing functions can't be directly used with strings
created by the functions this vocabulary provides, and these functions can not
be used freely with Retro strings.

For counted strings, you get a pointer to a structure in memory that looks like:

::

  length,characters

These can be unpacked into address/length pairs using **count**.


---------
Functions
---------
+-------------+----------+----------------------------------------------------+
| Function    | Stack    | Usage                                              |
+=============+==========+====================================================+
| if          | f-       | Start a conditional sequence                       |
+-------------+----------+----------------------------------------------------+
| else        | ``-``    | Start the second half of a conditional sequence    |
+-------------+----------+----------------------------------------------------+
| then        | ``-``    | End a conditional sequence                         |
+-------------+----------+----------------------------------------------------+
| 0<          | n-f      | Return true flag if n is less than zero            |
+-------------+----------+----------------------------------------------------+
| 0=          | n-f      | Return true flag if n is equal to zero             |
+-------------+----------+----------------------------------------------------+
| for         | n-       | Start a counted loop                               |
+-------------+----------+----------------------------------------------------+
| next        | ``-``    | End a counted loop                                 |
+-------------+----------+----------------------------------------------------+
| r@          | -n       | Return a copy of the top item on the address stack |
+-------------+----------+----------------------------------------------------+
| ``>r``      | n-       | Push a value to the address stack                  |
+-------------+----------+----------------------------------------------------+
| ``r>``      | -n       | Pop a value off the address stack                  |
+-------------+----------+----------------------------------------------------+
| 2dup        | xy-xyxy  | Duplicate the top two items on the stack           |
+-------------+----------+----------------------------------------------------+
| emit        | c-       | Display a character                                |
+-------------+----------+----------------------------------------------------+
| key         | -c       | Read a keypress                                    |
+-------------+----------+----------------------------------------------------+
| type        | an-      | Display n characters from string                   |
+-------------+----------+----------------------------------------------------+
| spaces      | n-       | Display a series of spaces                         |
+-------------+----------+----------------------------------------------------+
| state       | -a       | Same as **compiler**                               |
+-------------+----------+----------------------------------------------------+
| ``[']``     | "-a      | Return the address of a function. Compile-time     |
|             |          | version of **'**                                   |
+-------------+----------+----------------------------------------------------+
| recurse     | ``-``    | Compile a call to the current function into the    |
|             |          | function                                           |
+-------------+----------+----------------------------------------------------+
| ``*/mod``   | abc-rq   | Multiply a by b, then divide the results by c.     |
|             |          | Returns the remainder and the quotient.            |
+-------------+----------+----------------------------------------------------+
| rshift      | ab-c     | Shift bits right                                   |
+-------------+----------+----------------------------------------------------+
| lshift      | ab-c     | Shift bits left                                    |
+-------------+----------+----------------------------------------------------+
| fm/mod      | ab-mq    | Floored divide and remainder                       |
+-------------+----------+----------------------------------------------------+
| sm/mod      | ab-mq    | Symmetric divide and remainder                     |
+-------------+----------+----------------------------------------------------+
| ``c"``      | ``"-a``  | Parse and return a counted string                  |
+-------------+----------+----------------------------------------------------+
| ``s"``      | ``"-an`` | Parse and return a string and its length           |
+-------------+----------+----------------------------------------------------+
| count       | a-an     | Convert a counted string to an addr/len pair       |
+-------------+----------+----------------------------------------------------+
| ``[``       | ``-``    | Turn **compiler** off                              |
+-------------+----------+----------------------------------------------------+
| ``]``       | ``-``    | Turn **compiler** on                               |
+-------------+----------+----------------------------------------------------+
| char        | ``"-c``  | Parse for and return an ASCII character            |
+-------------+----------+----------------------------------------------------+
|``[char]``   | ``"-c``  | Compile-time version of **char**                   |
+-------------+----------+----------------------------------------------------+
| lateBinding | ``$-``   | "on" binds names to functions after execution of   |
|             |          | **;**, "off" binds immediately                     |
+-------------+----------+----------------------------------------------------+
| pick        |...n-..m  | Evil function to access arbitrary stack items      |
+-------------+----------+----------------------------------------------------+
| roll        | xu xu-1  | Remove u. Rotate u+1 items on the top of the stack |
|             | ... x0 u |                                                    |
|             | -- xu-1  |                                                    |
|             | ... x0 xu|                                                    |
+-------------+----------+----------------------------------------------------+


=====
hash'
=====


--------
Overview
--------
This library provides a vocabulary for generating hashes from strings.

Most documented hash functions leverage unsigned longs during computation.
Here we use *signed* cells as unsigned words are not offered by Retro at
the present time.  So that we avoid returning negative hash values, hashing
is filtered through **abs**.  (Negative values emerge due to shifting into
the sign bit.)

The **hash-prime** variable has been selected to provide a reasonable balance
between clashing and key size -- this is to ensure that associative arrays
built using **hash** don't need to allocate too much heap.  This can be
adjusted by revectoring **hash** in the [unlikely] event of large tables.


-------
Loading
-------
The following should suffice:

::

  needs hash'


--------
Examples
--------

::

  needs hash'
  "hello" ^hash'hash


---------
Functions
---------

+----------+-----------+---------------------------------+
| Name     | Stack     | Usage                           |
+==========+===========+=================================+
| djb2     | $-n       | Generate a djb2 hash. This is   |
|          |           | the default option.             |
+----------+-----------+---------------------------------+
| hash     | $-n       | Generate a hash from a string.  |
|          |           | Normally this is all you need to|
|          |           | use.                            |
+----------+-----------+---------------------------------+


---------------
Data Structures
---------------

+------------+-----------+---------------------------------+
| Name       | Stack     | Usage                           |
+============+===========+=================================+
| hash-prime |  -a       | Prime used by hashing algorithim|
+------------+-----------+---------------------------------+

======
infix'
======

--------
Overview
--------
This library provides a vocabulary for supporting simple infix math.

Expressions are processed from left to right.


Loading
=======
The following should suffice:

::

  needs infix'


Examples
========

::

  with infix'

  1 + 2 putn
  4 * 2 + 3 putn


Functions
=========

+----------+-----------+--------------+
| Name     | Stack     | Usage        |
+==========+===========+==============+
| ``+``    | ( x"-y  ) | ``1 + 2``    |
+----------+-----------+--------------+
| ``-``    | ( x"-y  ) | ``2 - 1``    |
+----------+-----------+--------------+
| ``*``    | ( x"-y  ) | ``3 * 3``    |
+----------+-----------+--------------+
| ``/``    | ( x"-y  ) | ``4 / 2``    |
+----------+-----------+--------------+
| ``mod``  | ( x"-y  ) | ``5 % 2``    |
+----------+-----------+--------------+
| ``/mod`` | ( x"-qr ) | ``5 /mod 2`` |
+----------+-----------+--------------+
| ``^``    | ( x"-n )  | ``5 ^ 2``    |
+----------+-----------+--------------+

==============
introspection'
==============

--------
Overview
--------
This vocabulary provides functions for examining functions and data structures.

---------
Functions
---------

+------------------+-------+----------------------------------------------------+
| Function         | Stack | Usage                                              |
+==================+=======+====================================================+
| isRevectorable?  | a-f   | Returns -1 if a function can be revectored, or 0   |
|                  |       | if not.                                            |
+------------------+-------+----------------------------------------------------+
| isVisible?       | a-f   | Returns -1 if a function is visible in the current |
|                  |       | dictionary, or 0 if not                            |
+------------------+-------+----------------------------------------------------+
| getClass         | a-a   | Return the class of a function, or 0 if not found  |
+------------------+-------+----------------------------------------------------+
| startOfBuffers   | -a    | Address where system buffers begin                 |
+------------------+-------+----------------------------------------------------+
| reserved         | -n    | Amount of memory used for buffers                  |
+------------------+-------+----------------------------------------------------+
| freeSpace        | -n    | Amount of free space (between **here** and         |
|                  |       | **startOfBuffers**                                 |
+------------------+-------+----------------------------------------------------+

===========
linkedList'
===========

--------
Overview
--------
While linked lists are a simple data structure, they are very useful.
This vocabulary provides a foundation for creating and using them in
a predicatable, consistent way.

The **linkedList'** vocabulary provides support for single linked
lists.


Back Linked Lists
=================
In this form a list consists of a series of nodes that each point to
the prior node. The list pointer variable is updated with each **add**
or **remove** to point to the newest node in the chain.


--------
Examples
--------

Creating A List
===============
::

  ^linkedList'new: L


Adding A Value To A List
========================
::

  100 L ^linkList'add


Removing A Value From A List
============================
::

  L ^linkList'remove


Accessing A Specific Node
=========================
::

  10 L ^linkList'nth


---------
Functions
---------

+--------------+-------+-----------------------------------------------+
| Name         | Stack | Usage                                         |
+==============+=======+===============================================+
| node         | "-    | Structure for list nodes                      |
+--------------+-------+-----------------------------------------------+
| .prior       | a-a   | Access prior node field                       |
+--------------+-------+-----------------------------------------------+
| .value       | a-a   | Access value field                            |
+--------------+-------+-----------------------------------------------+
| new:         | "-    | Create a list. This gives a variable pointing |
|              |       | to a **node** structure                       |
+--------------+-------+-----------------------------------------------+
| add          | nL-   | Add a value to a list. Allocates a new node   |
+--------------+-------+-----------------------------------------------+
| remove       | L-n   | Remove a node from a list and return its value|
+--------------+-------+-----------------------------------------------+
| addNode      | NL-   | Add a node to a list                          |
+--------------+-------+-----------------------------------------------+
| removeNode   | L-N   | Remove a node from a list. Returns the node   |
+--------------+-------+-----------------------------------------------+
| nth          | nL-N  | Return the *nth* node from the list. Indexing |
|              |       | starts at *1*.                                |
+--------------+-------+-----------------------------------------------+

=====
math'
=====
This library provides support for additional mathmatic operations not provided
by the core Retro language.


---------
Functions
---------
+------------+-------+-----------------------------------------------------+
| Function   | Stack | Description                                         |
+============+=======+=====================================================+
| squareRoot | x-n   | Find an (approximate) square root for a given value |
+------------+-------+-----------------------------------------------------+
| gcd        | xy-n  | Find the greatest common denominator for two values |
+------------+-------+-----------------------------------------------------+
| lcm        | xy-n  | Find the least common multiplier for two values     |
+------------+-------+-----------------------------------------------------+
| divisor?   | ab-f  | Tests to see if b is a divisor of a. Returns a flag.|
+------------+-------+-----------------------------------------------------+
| ``*/``     | abc-d | Multiply a by b, then divide bye c to get d         |
+------------+-------+-----------------------------------------------------+
| even?      | n-f   | Returns a flag indicating whether or not a number is|
|            |       | true                                                |
+------------+-------+-----------------------------------------------------+
| odd?       | n-f   | Returns a flag indicating whether or not a number is|
|            |       | false                                               |
+------------+-------+-----------------------------------------------------+
| pow        | bp-n  |  Raise (b) to power (p)                             |
+------------+-------+-----------------------------------------------------+
| abs        |  n-n  |  Absoulte value of number (n)                       |
+------------+-------+-----------------------------------------------------+
| min        | ab-c  |  Minimum of (a) or (b)                              |
+------------+-------+-----------------------------------------------------+
| max        | ab-c  |  Maximum of (a) or (b)                              |
+------------+-------+-----------------------------------------------------+
| random     |   -x  |  Return a random number                             |
+------------+-------+-----------------------------------------------------+


========
queries'
========

--------
Overview
--------

This library provides symbolic constants and helper functions for
using the VM queries in a clean, portable manner.


---------
Functions
---------

+------------------------+-----+------------------------------+
| MEMORY-SIZE            | -n  | Query for determining amount |
|                        |     | of memory provided by the    |
|                        |     | virtual machine              |
+------------------------+-----+------------------------------+
| CANVAS?                | -n  | Query for determining whether|
|                        |     | the virtual machine provides |
|                        |     | a canvas device              |
+------------------------+-----+------------------------------+
| CANVAS-WIDTH           | -n  | Query returning canvas width |
+------------------------+-----+------------------------------+
| CANVAS-HEIGHT          | -n  | Query returning canvas height|
+------------------------+-----+------------------------------+
| STACK-DEPTH            | -n  | Query returning data stack   |
|                        |     | depth                        |
+------------------------+-----+------------------------------+
| ADDRESS-STACK-DEPTH    | -n  | Query returning address stack|
|                        |     | depth                        |
+------------------------+-----+------------------------------+
| MOUSE?                 | -n  | Query for determining if the |
|                        |     | virtual machine provides a   |
|                        |     | mouse device                 |
+------------------------+-----+------------------------------+
| TIME                   | -n  | Query returning current time |
|                        |     | in seconds                   |
+------------------------+-----+------------------------------+
| QUIT-VM                | -n  | Query to shutdown the virtual|
|                        |     | machine                      |
+------------------------+-----+------------------------------+
| HOST-ENVIRONMENT-QUERY | -n  | Query to lookup and return   |
|                        |     | the value of an environment  |
|                        |     | variable in the host system  |
+------------------------+-----+------------------------------+
| CONSOLE-WIDTH          | -n  | Query returning the console  |
|                        |     | width                        |
+------------------------+-----+------------------------------+
| CONSOLE-HEIGHT         | -n  | Query returning the console  |
|                        |     | height                       |
+------------------------+-----+------------------------------+
| BITS-PER-CELL          | -n  | Query returning the number of|
|                        |     | bits in a cell               |
+------------------------+-----+------------------------------+
| ENDIAN                 | -n  | Query returning a flag which |
|                        |     | indicates endian used by the |
|                        |     | virtual machine              |
+------------------------+-----+------------------------------+
| CONSOLE?               | -n  | Query to see if enhanced text|
|                        |     | console is provided          |
+------------------------+-----+------------------------------+
| query                  | ?-? | Perform a query. Actual stack|
|                        |     | effect varies by query       |
+------------------------+-----+------------------------------+


======
stack'
======
This library provides a vocabulary for creating and using custom LIFO stacks.


-------
Loading
-------
The following should suffice:

::

  needs stack'


--------
Examples
--------

::

  with stack'

  new: foo
  100 push
  200 push

  new: bar
  300 push
  400 push
  500 push

  depth putn
  pop putn

  foo set
  depth putn
  pop putn
  pop putn

  bar set
  pop putn
  pop putn


---------
Functions
---------

+----------+-----------+---------------------------------+
| Name     | Stack     | Usage                           |
+==========+===========+=================================+
| get      |  -a       | Get address of current stack    |
+----------+-----------+---------------------------------+
| set      | a-        | Activate a stack                |
+----------+-----------+---------------------------------+
| new      | ``-``     | Create a new anonymous stack    |
+----------+-----------+---------------------------------+
| new:     | ``"-``    | Create a new named stack        |
+----------+-----------+---------------------------------+
| push     | n-        | Push a value to a stack         |
+----------+-----------+---------------------------------+
| pop      | -n        | Pop a value from a stack        |
+----------+-----------+---------------------------------+
| depth    | -n        | Return number of items on stack |
+----------+-----------+---------------------------------+

=======
struct'
=======


--------
Overview
--------
This vocabulary provides support for creating and using simple data
structures.


-------
Example
-------
::

  with struct'

  {
    1 field .id
    4 field .initials
    1 field .jobcode
  } employee

  ( The above could also be written as: )
  { 3 fields .id 4 .initials .jobcode } employee

  employee Fred
  1 Fred !.id
  "fjh" Fred .initials 3 copy
  22 Fred !.jobcode

  employee Sue
  2 Sue !.id
  "sjs" Sue .initials 3 copy
  91 Sue !.jobcode

  Fred @.id putn
  Sue @.id putn
  Sue .initials puts


---------
Functions
---------

+-------+-------+----------------------------------------------------------+
| Name  | Stack | Used for                                                 |
+=======+=======+==========================================================+
| {     | -n    | Start a new structure                                    |
+-------+-------+----------------------------------------------------------+
| field | nn"-n | Add a field to a structure. Takes the number of cells to |
|       |       | allocate and parses for a name                           |
+-------+-------+----------------------------------------------------------+
| fields| n"-n  | Add multiple fields to a structure. Takes the number of  |
|       |       | fields and parses for names and optionally sizes.        |
+-------+-------+----------------------------------------------------------+
| }     | n"-   | End a structure. Parses for the structure name           |
+-------+-------+----------------------------------------------------------+
| size  | a-n   | Return the size of a structure. The address must be the  |
|       |       | address of the function created by **}**, not the actual |
|       |       | allocated structure.                                     |
+-------+-------+----------------------------------------------------------+
| clone | a-a   | Given a structure address, return a new anonyous instance|
+-------+-------+----------------------------------------------------------+

========
subcell'
========


--------
Overview
--------
This library provides functions for accessing 16-bit and 8-bit subsets of cells.


---------
Functions
---------
+-----------------+-------------------------------+
| Function        | Stack                         |
+=================+===============================+
| highWord        | cellAddr -- highwordAddr      |
+-----------------+-------------------------------+
| lowWord         | cellAddr -- lowwordAddr       |
+-----------------+-------------------------------+
| w@              | wordAddr -- val               |
+-----------------+-------------------------------+
| w@+             | wordAddr -- wordAddr val      |
+-----------------+-------------------------------+
| w!              | val wordAddr --               |
+-----------------+-------------------------------+
| w!+             | val wordAddr -- wordAddr+1    |
+-----------------+-------------------------------+
| highByte        | wordAddr -- highbyteAddr      |
+-----------------+-------------------------------+
| lowByte         | wordAddr -- lowbyteAddr       |
+-----------------+-------------------------------+
| c@              | byteAddr -- val               |
+-----------------+-------------------------------+
| c@+             | byteAddr -- byteAddr val      |
+-----------------+-------------------------------+
| c!              | val byteaddr --               |
+-----------------+-------------------------------+
| c!+             | val byteAddr -- byteAddr+1    |
+-----------------+-------------------------------+
| highNibble      | byteAddr -- highnibbleAddr    |
+-----------------+-------------------------------+
| lowNibble       | byteAddr -- lownibbleAddr     |
+-----------------+-------------------------------+
| nibble@         | nibbleAddr -- val             |
+-----------------+-------------------------------+
| nibble@+        | nibbleAddr -- nibbleAddr val  |
+-----------------+-------------------------------+
| nibble!         | val nibbleAddr --             |
+-----------------+-------------------------------+
| nibble!+        | val nibbleAddr -- nibbleAddr+1|
+-----------------+-------------------------------+
| packString      | strAddr --                    |
+-----------------+-------------------------------+
| printPack       | strAddr --                    |
+-----------------+-------------------------------+
| getPackedLength | strAddr -- strLen             |
+-----------------+-------------------------------+


=========
unsigned'
=========

--------
Overview
--------
This vocabulary will provide support for creation and use of unsigned values.



=======
values'
=======
A value is a variable that returns the stored value when used. This
library provides an implementation for Retro.


-------
Loading
-------
The following should suffice:

::

  needs values'


--------
Examples
--------

::

  with values'

  value foo

  100 to foo

  foo foo + .s

  50 to foo
  foo foo + .s


---------
Functions
---------

+----------+-----------+-------------------------+
| Name     | Stack     | Usage                   |
+==========+===========+=========================+
| .value   | | a-n     | | Return a stored value |
|          | | *or*    | |                       |
|          | | na-     | | Update a stored value |
+----------+-----------+-------------------------+
| to       | ```-``    | Switch value to update  |
|          |           | mode                    |
+----------+-----------+-------------------------+
| value    | "-        | Create a new value      |
+----------+-----------+-------------------------+

===========
variations'
===========

--------
Overview
--------
This library provides some functions for determining the differences between
VM implementations and conditionally evaluating code blocks based on these
differences.

Specifically, it provides functions for determining the number of bits per
cell and the endian nature of an implementation.


---------
Functions
---------

+--------------+-------+------------------------------------------------+
| Function     | Stack | Usage                                          |
+==============+=======+================================================+
| size         | -n    | Return the number of bits per cell             |
+--------------+-------+------------------------------------------------+
| endian       | -n    | Return 0 for little endian or 1 for big endian |
+--------------+-------+------------------------------------------------+
| bits{        | n"-   | Execute code up to **}** if the bits per cell  |
|              |       | matches *n*                                    |
+--------------+-------+------------------------------------------------+
| bigEndian    | "-    | Execute code up to **}** if endian is big      |
+--------------+-------+------------------------------------------------+
| littleEndian | "-    | Execute code up to **}** if endian is little   |
+--------------+-------+------------------------------------------------+
| }            | ``-`` | Does nothing                                   |
+--------------+-------+------------------------------------------------+

