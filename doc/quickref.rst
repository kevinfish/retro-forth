==============================
Retro Language Quick Reference
==============================

-----------------
Displaying Things
-----------------

Numbers
=======

::

  100 putn


Characters
==========

::

  'a putc


Strings
=======

::

  "hello, world" puts


Mixed
=====

::

  1 2 3 "%d - %d = %d\n" puts
  "Charles"  "Hello %s\n" puts


-----
Input
-----

Characters
==========

::

  getc


Strings
=======

Standard
--------

::

  "this is a string"
  "   this string has leading and trailing whitespace   "


Whitespace Delimited
--------------------

::

  getToken


Character Delimited
-------------------

::

  'c accept tib


Numbers
=======

::

  getNumber


------------
Conditionals
------------

Equality
========

::

  2 3 =


Inequality
==========

::

  3 4 <>


Greather Than
=============

::

  3 4 >


Less Than
=========

::

  3 4 <


Greater Than or Equal To
========================

::

  3 4 >=


Less Than or Equal To
=====================

::

  3 4 <=


String Equality
===============

::

  "hello"   "world"  compare


String Inequality
=================

::

  "hello"   "world"  compare not


If / Then
=========

::

  1 2 = [ "true" ] [ "false" ] if
  1 2 = [ "true" ] ifTrue
  1 2 = [ "false" ] ifFalse


Multiple Comparisons
====================

::

  [ @base
    [  8 = ] [ "octal"       ] whend
    [ 10 = ] [ "decimal"     ] whend
    [ 16 = ] [ "hexadecimal" ] whend
    drop ( default case ) ] do

  [ @base
    [  8 = ] [ "base is octal: %d"   puts ] when
    [ 10 = ] [ "base is decimal: %d" puts ] when
    drop ( default case ) ] do


-------------------
Function Defintions
-------------------

Quotes (Anonymous)
==================

::

  [  ( code )  ]

Quotes can be nested.


Named
=====

::

  : name  ( stack comment )
    ( code ) ;

Quotes can be nested inside a named function.


-----
Loops
-----

Unconditional
=============

::

  repeat ( code ) again

Conditional loops are generally preferable.


Counted
=======

::

  ( simple, no index on stack )
  10 [ 'a putc ] times

  ( index on stack, counts up )
  10 [ putn ] iter

  ( index on stack, counts down )
  10 [ putn ] iterd


Conditional
===========

::

  10 [ 1- dup putn dup 0 <> ] while


----
Math
----

Addition
========

::

  100 200 +


Subtraction
===========

::

  400 32 -


Multiplication
==============

::

  98 12 *


Division
========

::

  200 4 /


Remainder
=========

::

  203 4 mod


Power
=====

Raise 3 to the second power.

::

  3 2 pow


Absolute Value
==============

::

  -76 abs


Minimum and Maximum Value
=========================

::

  34 8 min
  34 8 max


------------
Vocabularies
------------

Creation
========

::

  chain: name'
    ... contents ...
  ;chain

Vocabulary names should end in a single apostrophe.


Add To Search Order
===================

::

  with name'
  with| name' and' more' names' |


Remove From Search Order
========================

::

  ( remove the most recently added vocabulary )
  without

  ( remove all vocabularies )
  global


Access a Function in a Vocabulary
=================================

::

  ^vocabulary'function

