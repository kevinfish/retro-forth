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

