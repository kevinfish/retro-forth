============
combinators'
============


--------
Overview
--------
This library serves as a home to various combinators that are useful, but not
essential to the core Retro language.


-------
Loading
-------
::

  needs combinators'


---------
Functions
---------
+----------+---------+------------------------------------------------+
| Function | Stack   | Used For                                       |
+==========+=========+================================================+
| loopd    | seq-    | Execute a loop. Index starts at high value (e) |
|          |         | and decrements to low value (s). The loop body |
|          |         | is quote (q).                                  |
+----------+---------+------------------------------------------------+
| indexd   | -n      | Return the current index for a decrementing    |
|          |         | loop.                                          |
+----------+---------+------------------------------------------------+
| loopi    | seq-    | Execute a loop. Index starts at low value (s)  |
|          |         | and increments to high value (e). The loop body|
|          |         | is quote (q).                                  |
+----------+---------+------------------------------------------------+
| indexi   | -n      | Return the current index for an incrementing   |
|          |         | loop.                                          |
+----------+---------+------------------------------------------------+

