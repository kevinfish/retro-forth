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

