=============
Release Notes
=============

----------
Retro 11.5
----------

core language / standard image
==============================

  - metacompiler changes

    - docstring support
    - create headers during metacompilation

  - clean ups and refactorings

    - increased use of & prefix over [ ]
    - comments on quotation compiler added (tangentstorm)

  - new functionality

    - added yield
    - added d->doc
    - added describe
    - added :needs
    - added libraries
    - added parable style conditionals (now default)
    - exposed getc:unfiltered
    - keymap support

  - moved functions to internal' vocabulary

    - quote
    - string

  - docstrings now part of standard image

    - make "smallimage" to get one without them


libraries
=========

  - dissect' now tries to display function names for calls
  - added havn hashes from Mat2 to hash'


examples
========

  - autopsy

    - added words:vectored
    - added words:byClass
    - use latest dissect'

  - block editor

    - fixed a few issues causing corruption (thanks to docl finding the bugs)

  - assembler


documentation
=============

  - various fixes


vm
==

  - improved python implementation from tangentstorm


----------
Retro 11.4
----------

Compatibility Issues
====================

This release brings several changes that may require small alterations to
existing source code.

First, the **files'** vocabulary is now in the library and not the core
language. For most users, adding a single line to the start of their sources
will be sufficient.

::

  needs files'

The second change is more significant. We have moved **pow**, **abs**, **min**,
**max**, and **random** to the **math'** library. If you have source using these,
you need to add the following to your sources:

::

  needs math'
  with  math'

If you don't want to do **with math'**, you can add a **^math'** prefix to each
use of these functions and just use the **needs math'** line.

Third, hidden headers are no longer preserved. There is a new function, **HEADERS**,
which returns the maximum number of hidden headers. Exceeding this will cause
problems, so it can be revectored to increase (or decrease) as needed.

Fourth, the **tib**, temporary string buffers, and hidden headers are now moved
to the end of physical memory. Use the **introspection'** library to determine
the size and actual locations of these buffers, and avoid overflowing into them.
E.g., in **casket'**, we use **^introspection'startOfBuffers** to identify the
start of the buffers and dynamically shift the casket buffers to a safe location.

Fifth, string constants are now created using **string:** instead of **string**.
This requires a quick search/replace to fix.


core language / standard image
==============================

  - clean ups and refactorings

    - **{{** and **}}**
    - **:is** and **:devector**
    - **each@**
    - **<puts>**
    - **toNumber**

  - removals (now in library)

    - **pow**
    - **abs**
    - **min**
    - **max**
    - **random**
    - **files'**

  - new memory layout (buffers at physical end of memory)
  - hidden headers now disappear when global scope resumes
  - helper function **string** for strings


libraries
=========

  - **d'** renamed to **decimal'** (resolve naming conflict)
  - add **queries'**
  - add **dump'**
  - add **fixed'**
  - add **double'**
  - add **unsigned'**
  - add **introspection'**
  - add **files'** (used to be in core image)
  - update **math'**

    - add **pow** (used to be in core image)
    - add **abs** (used to be in core image)
    - add **min** (used to be in core image)
    - add **max** (used to be in core image)
    - add **random** (used to be in core image)

  - updated **console'** to use new console device with VT100 fallback


examples
========

  - cleanups to make better use of higher-level language features

vm
==

  - add retro-curses
  - C# is now feature complete
  - added support for VM-level console device
  - new java implementation from Mike Andrews

website
=======

  - fix broken links





----------
Retro 11.3
----------


Compatibility Issues
====================

This release of Retro brings a number of changes that may require small
alterations to existing sources. Specifically:

  - maximum string length is now 256 cells by default
  - hidden functions are no longer revectorable

The first is not a big deal. It's now possible to alter both the maximum
string length and the number of temporary buffers. To restore a 512 cell
size as in 11.2:

::

  here 512 allot constant <512-TIB>
  [ <512-TIB> ] is tib
  [ 512 ] is STRING-LENGTH

The second change shouldn't be too critical. Basically it means that this
will no longer work:

::

  {{
    : foo  ( - ) 50 ;
  ---reveal---
    : bar1  foo foo + ;
    [ 100 ] is foo
  }}

Any private definitions are no longer revectorable. If you have no
**---reveal---** in a namespace, all functions are non-revectorable. This
won't break anything in the standard language or libraries, but could
be a problem if you rely on the old behavior in your code.


core language / standard image
==============================

  - new method of implementing quotes
  - maximum string length can be altered now
  - number of string buffers can be altered now
  - internal factors in kernel are no longer revectorable
  - removed use of low level conditionals outside of the kernel
  - reduced amount of padding in kernel
  - reduced default string length to 256 cells
  - metacompiler now strips unused space at end of kernel
  - reduced image size to under 9k cells
  - added until loop combinator
  - hidden functions are no longer revectorable


libraries
=========

  - fixed all reported bugs
  - added diet' library for reducing memory usage by trimming string size, buffers
  - added fiction' library for simple interactive fiction games
  - documentation blocks have consistent formatting now


extensions
==========

  - include lua bindings (now updated for the lua 5.2 release)
  - include sqlite bindings


ngaro vm
========

  - c

    - add --help flag
    - load image from $RETROIMAGE environment variable if not found in working directory
    - added variant for Windows users (can be built with tcc)

  - golang

    - updated to work with current weekly builds
    - added README to explain how to build it and cover concurrency additions

  - html5

    - use bootstrap for ui elements

  - php

    - fixed bugs in host environment queries, now feature complete

  - embedded

    - support pic32 boards running RetroBSD
    - updates to arduino implementation from Oleksandr

      - now supports MEGA 2560 and Nano boards

    - slightly lower memory usage in mbed implementation


examples
========

  - fixed bugs
  - updated to use new language features
  - added bingo card generator
  - added hex dump utility
  - added tab completion example (from Luke)
  - added example of building strings using a combinator


documentation
=============

  - add single file covering all of the libraries
  - expansions to quick reference
  - minor updates to fix small mistakes, clarify things


other
=====

  - properly support multi-line strings in vim highlighter
  - the debugger now has a source display view

