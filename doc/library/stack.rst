======
stack'
======
This library provides a vocabulary for creating and using custom LIFO stacks.


-------
Loading
-------
The following should suffice:

::

  needs stack'


--------
Examples
--------

::

  with stack'

  new: foo
  100 push
  200 push

  new: bar
  300 push
  400 push
  500 push

  depth putn
  pop putn

  foo set
  depth putn
  pop putn
  pop putn

  bar set
  pop putn
  pop putn


---------
Functions
---------

+----------+-----------+---------------------------------+
| Name     | Stack     | Usage                           |
+==========+===========+=================================+
| get      |  -a       | Get address of current stack    |
+----------+-----------+---------------------------------+
| set      | a-        | Activate a stack                |
+----------+-----------+---------------------------------+
| new      | ``-``     | Create a new anonymous stack    |
+----------+-----------+---------------------------------+
| new:     | ``"-``    | Create a new named stack        |
+----------+-----------+---------------------------------+
| push     | n-        | Push a value to a stack         |
+----------+-----------+---------------------------------+
| pop      | -n        | Pop a value from a stack        |
+----------+-----------+---------------------------------+
| depth    | -n        | Return number of items on stack |
+----------+-----------+---------------------------------+

