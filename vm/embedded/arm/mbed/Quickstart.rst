================
Quickstart Guide
================

-------------
Prerequisites
-------------

* mbed LPC1768
* computer
* vm binary
* image
* putty


----------
Setting Up
----------
Plug the mbed into the computer. When the mass storage device shows up copy the
VM and image to it.

Launch Putty. Select "serial" connection, enter the COM port used by the mbed.
Connect.

Push the reset button. You will now be running Retro.


----------
Testing It
----------

Enter this:

::

  with mbed'

  : on     ( n- )  1 swap p! ;
  : off    ( n- )  0 swap p! ;
  : delay  ( n- )  time 1+ [ time over <> ] while drop ;
  : wait   (  - )  1 delay ;

  : demo   (  - )
    LED1 on wait
    LED2 on wait
    LED3 on wait
    LED4 on wait
    LED1 off wait
    LED2 off wait
    LED3 off wait
    LED4 off wait ;

  demo

The four LED should light up and then turn off.


-----------
Limitations
-----------
* 32 cells for data stack
* 128 cells for address stack
* Filenames max at 80 chars (including terminator)
* Max of 4 open files
* Filenames must have a /local/ prefix

