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

