Binary image layout:

000  character  P  \ Image type identifier
001  character  I  / "parable image"
002  integer    number of headers
003  integer    number of slots
004  integer    cells per slot
005+ headers
     - ptrs to names
     - slot #
     - type hint
     slot usage table
     memory slots

First addressable slot is 0.

