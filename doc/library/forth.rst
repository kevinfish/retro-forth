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


