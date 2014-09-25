=====
hash'
=====


--------
Overview
--------
This library provides a vocabulary for generating hashes from strings.

Most documented hash functions leverage unsigned longs during computation.
Here we use *signed* cells as unsigned words are not offered by Retro at
the present time.  So that we avoid returning negative hash values, hashing
is filtered through **abs**.  (Negative values emerge due to shifting into
the sign bit.)

The **hash-prime** variable has been selected to provide a reasonable balance
between clashing and key size -- this is to ensure that associative arrays
built using **hash** don't need to allocate too much heap.  This can be
adjusted by revectoring **hash** in the [unlikely] event of large tables.


-------
Loading
-------
The following should suffice:

::

  needs hash'


--------
Examples
--------

::

  needs hash'
  "hello" ^hash'hash


---------
Functions
---------

+----------+-----------+---------------------------------+
| Name     | Stack     | Usage                           |
+==========+===========+=================================+
| djb2     | $-n       | Generate a djb2 hash. This is   |
|          |           | the default option.             |
+----------+-----------+---------------------------------+
| hash     | $-n       | Generate a hash from a string.  |
|          |           | Normally this is all you need to|
|          |           | use.                            |
+----------+-----------+---------------------------------+


---------------
Data Structures
---------------

+------------+-----------+---------------------------------+
| Name       | Stack     | Usage                           |
+============+===========+=================================+
| hash-prime |  -a       | Prime used by hashing algorithim|
+------------+-----------+---------------------------------+

