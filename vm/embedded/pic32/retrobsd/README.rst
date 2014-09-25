==================
Retro for RetroBSD
==================

--------
Overview
--------
This is an implementation of Retro for use with RetroBSD.


-----------
Constraints
-----------
This build uses a 32-bit image and provides 21k cells of memory. The
default image used is using diet' to reduce the length of strings.


----------
Setting Up
----------
Run *make*. This will generate an image suitable for low-memory devices
and place it in the **retro-src** directory. Copy the **retro-src**
directory to the **src** directory in your RetroBSD source tree. Switch
to that, then run **make**.

