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

