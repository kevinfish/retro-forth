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

