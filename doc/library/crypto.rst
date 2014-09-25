=======
crypto'
=======

--------
Overview
--------
I enjoy dabbling with logic puzzles and classical forms of encryption.
This vocabulary is intended to be a tool to help with these diversions.


---------
Functions
---------

+-----------------+-------+----------------------------------------------------+
| Name            | Stack | Usage                                              |
+=================+=======+====================================================+
| ceaser          | $n-$  | Encrypt (or decrypt) a string using the ceaser     |
|                 |       | algorithm. The value n is the number of            |
|                 |       | characters to shift the alphabet by.               |
+-----------------+-------+----------------------------------------------------+
| rot13           | $-$   | Encrypt or decrypt a string using rot13. This      |
|                 |       | is a trivial variant of the ceaser algorithm.      |
+-----------------+-------+----------------------------------------------------+
| tapcode:encrypt | $-a   | Encrypt a string into a tapcode array              |
+-----------------+-------+----------------------------------------------------+
| tapcode:decrypt | a-$   | Decrypt a tapcode array into a string              |
+-----------------+-------+----------------------------------------------------+
| pigpen:encrypt  | $-a   | Convert a string into a pigpen encoded array.      |
|                 |       | To display the pigpen sequence, try:               |
|                 |       |                                                    |
|                 |       | ::                                                 |
|                 |       |                                                    |
|                 |       |   "ab" ^crypto'toPigpen [ puts space ] ^array'apply|
+-----------------+-------+----------------------------------------------------+
| pigpen:decrypt  | a-$   | Decode a pigpen encoded array into a string        |
+-----------------+-------+----------------------------------------------------+
| keyword:setKey  | $-    | Set the key to use for keyword encryption. This    |
|                 |       | should be 26 characters long.                      |
+-----------------+-------+----------------------------------------------------+
| keyword:encrypt | $-$   | Encrypt a string using keyword encryption          |
+-----------------+-------+----------------------------------------------------+
| keyword:decrypt | $-$   | Decrypt a string using keyword encryption          |
+-----------------+-------+----------------------------------------------------+

