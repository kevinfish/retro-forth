==========
decompose'
==========


--------
Overview
--------
The core Casket framework provides minimal templating, but no easy way to
generate HTML from code.


This vocabulary provides support for generating HTML using a series of
combinators. Textual data returned will be displayed (using **^casket'tputs**),
and various attributes are supported.


-------
Loading
-------
::

  needs decompose'


-------
Example
-------
::

  : /index
    Content-type: text/html
    [ [ [ "Hello" ] title ] head
      [ [ "Welcome" ] h1
        [ "This is a paragraph." ] p ] body ] html ;


