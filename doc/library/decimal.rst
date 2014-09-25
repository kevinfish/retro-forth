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

