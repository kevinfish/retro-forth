=======
casket'
=======

--------
Overview
--------
Casket is a lightweight framework intended to simplify the development of
web apps in Retro.

Casket grew out of my work on the Corpse blog and later through work on the
Rancid IRC log viewer.


---------
Functions
---------
+------------------+-------+--------------------------------------------------+
| Name             | Stack | Usage                                            |
+==================+=======+==================================================+
| serve:           | """-  | Serve a file with a specific mime type           |
+------------------+-------+--------------------------------------------------+
| decode           | $-    | Decode a URL encoded string                      |
+------------------+-------+--------------------------------------------------+
| dispatch         | ``-`` | Look for a view handler (e.g., /index) and call  |
|                  |       | it, or call **/404** if none is found            |
+------------------+-------+--------------------------------------------------+
| doBeforeDispatch | ``-`` | Code to execute before processing paths. This is |
|                  |       | always called before **dispatch**.               |
+------------------+-------+--------------------------------------------------+
| /                | ``-`` | Default index page                               |
+------------------+-------+--------------------------------------------------+
| /404             | ``-`` | Default 404 error page                           |
+------------------+-------+--------------------------------------------------+
| withTemplate     | $-    | Include and render a template file using the     |
|                  |       | **tputs** function                               |
+------------------+-------+--------------------------------------------------+
| withFile         | $-    | Include and render a raw file                    |
+------------------+-------+--------------------------------------------------+
| tputs            | ...$- | This is a replacement for **puts**, which adds   |
|                  |       | additional escape sequences for use with the     |
|                  |       | templates.                                       |
+------------------+-------+--------------------------------------------------+
| eputs            | $-    | This is a replacement for **puts**, which escapes|
|                  |       | html entities for use with <pre> and such        |
+------------------+-------+--------------------------------------------------+
| Content-type:    | "-    | Generate a MIME header for a file or view        |
+------------------+-------+--------------------------------------------------+
| getFormData      | -$    | Process a form and return the elements as a text |
|                  |       | string                                           |
+------------------+-------+--------------------------------------------------+
| getRequest       | ``-`` | Internal: Read the PATH_INFO environment variable|
|                  |       | and parse for **casket:path** and                |
|                  |       | **casket:options**                               |
+------------------+-------+--------------------------------------------------+
| casket:url       | -a    | Function returning application url               |
+------------------+-------+--------------------------------------------------+
| casket:root      | -a    | Function returning path to application root      |
+------------------+-------+--------------------------------------------------+
| casket:form      | -a    | Internal buffer for form data                    |
+------------------+-------+--------------------------------------------------+
| casket:buffer    | -a    | Internal buffer                                  |
+------------------+-------+--------------------------------------------------+
| casket:options   | -a    | Returns optional data following view)            |
+------------------+-------+--------------------------------------------------+
| casket:path      | -a    | Returns requested view (e.g., /index)            |
+------------------+-------+--------------------------------------------------+


-------
Example
-------

application.rx
==============
::

  needs casket'
  with casket'

 : /index
    Content-type: text/html
    "index.erx" withTemplate ;

  : /test
    Content-type: text/html
    casket:options "test.erx" withTemplate ;

  [ ( -$ ) "/path/to/app/" ] is casket:root
  [ ( -$ ) "http://domain.ext/path/to/cgi" ] is casket:url
  &/index is /
  &dispatch is boot
  save bye


index.erx
=========
::

  <html>
  <body><h1>Test of Casket</h1>
  <p><a href='%u/test/apple'>test apple</a>
     or <a href='%u/test/banana'>test banana</a>.</p>
  </body></html>


test.erx
========
::

  <html>"  tputs
  <body><h1>You requested...</h1>
  <p>An %s.</p>
  </body></html>


