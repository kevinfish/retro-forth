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

