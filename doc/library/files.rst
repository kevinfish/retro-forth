======
files'
======

.. class:: corefunc

+-----------------+-----------+-----------------------------------------------+
| Function        | Stack     | Notes                                         |
+=================+===========+===============================================+
|   :R            |     -n    |  Mode for reading a file. Does not create file|
+-----------------+-----------+-----------------------------------------------+
|   :W            |     -n    |  Mode for writing a file                      |
+-----------------+-----------+-----------------------------------------------+
|   :A            |     -n    |  Mode for appending to a file                 |
+-----------------+-----------+-----------------------------------------------+
|   :M            |     -n    |  Mode for modifying a file. Does not create   |
|                 |           |  file.                                        |
+-----------------+-----------+-----------------------------------------------+
|   open          |   $m-h    |  Open a file. Will return a handle. Valid     |
|                 |           |  handles will be non-zero. A zero handle      |
|                 |           |  indicates failure to open a file.            |
+-----------------+-----------+-----------------------------------------------+
|   read          |    h-c    |  Read a byte from a file. This returns the    |
|                 |           |  byte.                                        |
+-----------------+-----------+-----------------------------------------------+
|   write         |   ch-f    |  Write a byte to a file. This returns a flag  |
|                 |           |  indicating the number of bytes written.      |
|                 |           |  (Should always equal '1')                    |
+-----------------+-----------+-----------------------------------------------+
|   close         |    h-f    |  Close an open file. Returns a flag of zero if|
|                 |           |  unable to close, or non-zero if successful.  |
+-----------------+-----------+-----------------------------------------------+
|   pos           |    h-n    |  Get current position in a file               |
+-----------------+-----------+-----------------------------------------------+
|   seek          |   nh-f    |  Seek a position in a file                    |
+-----------------+-----------+-----------------------------------------------+
|   size          |    h-n    |  Return size of open file                     |
+-----------------+-----------+-----------------------------------------------+
|   delete        |    $-f    |  Delete a file. Returns a handle. Non-zero if |
|                 |           |  successful, zero if failed.                  |
+-----------------+-----------+-----------------------------------------------+
|   slurp         |   a$-n    |  Read a file into a buffer                    |
+-----------------+-----------+-----------------------------------------------+
|   spew          |  an$-n    |  Write (n) bytes from address (a) into a file |
+-----------------+-----------+-----------------------------------------------+
|   readLine      |    h-$    |  Read a line from a file                      |
+-----------------+-----------+-----------------------------------------------+
|   writeLine     |   $h-     |  Write a string to a file                     |
+-----------------+-----------+-----------------------------------------------+

