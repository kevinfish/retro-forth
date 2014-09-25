====
bad'
====


--------
Overview
--------
The Ngaro virtual machine underlying Retro does not provide access to memory
in units other than a standard cell. This simplifies things in many ways, but
can be very wasteful of memory, especially when dealing with large amounts of
text. This library provides support for dealing with byte-level access to the
Ngaro memory.


-----------
Terminology
-----------
*byte*: A unit of memory consisting of eight bits.

*cell*: A unit of memory consisting of thirty two bits.


-------
Loading
-------
::

  needs bad'


-------
Example
-------
::

  1024 ^bad'newPool
  'e 0 ^bad'b!
  'f 1 ^bad'b!
  'g 2 ^bad'b!
  'h 3 ^bad'b!
  ^bad'pool @ putn
  2 ^bad'b@ putn


---------
Functions
---------
+----------+--------+------------------------------------------------+
| Function | Stack  | Used For                                       |
+==========+========+================================================+
| pool     | -a     | Variable. This holds a pointer to the current  |
|          |        | byte addressable memory pool                   |
+----------+--------+------------------------------------------------+
| unpack   | c-bbbb | Given a byte-packed cell on the stack return   |
|          |        | the bytes it contains                          |
+----------+--------+------------------------------------------------+
| pack     | bbbb-c | Pack four byes into a cell, return the cell on |
|          |        | the stack                                      |
+----------+--------+------------------------------------------------+
| b@       | a-b    | Fetch a byte from the active byte addressable  |
|          |        | memory pool                                    |
+----------+--------+------------------------------------------------+
| b!       | ba-    | Store a byte into the currently active byte    |
|          |        | addressable memory pool                        |
+----------+--------+------------------------------------------------+
| b@+      | a-ab   | Fetch a byte from the active byte addressable  |
|          |        | memory pool. Returns next byte address and the |
|          |        | byte fetched                                   |
+----------+--------+------------------------------------------------+
| b!+      | ba-a   | Store a byte into the currently active byte    |
|          |        | addressable memory pool. Returns next byte     |
|          |        | address                                        |
+----------+--------+------------------------------------------------+
| newPool  |  n-    | Allocate a new pool of *n* characters and set  |
|          |        | **pool** to point to it                        |
+----------+--------+------------------------------------------------+

