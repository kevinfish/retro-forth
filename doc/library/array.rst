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

