=======
struct'
=======


--------
Overview
--------
This vocabulary provides support for creating and using simple data
structures.


-------
Example
-------
::

  with struct'

  {
    1 field .id
    4 field .initials
    1 field .jobcode
  } employee

  ( The above could also be written as: )
  { 3 fields .id 4 .initials .jobcode } employee

  employee Fred
  1 Fred !.id
  "fjh" Fred .initials 3 copy
  22 Fred !.jobcode

  employee Sue
  2 Sue !.id
  "sjs" Sue .initials 3 copy
  91 Sue !.jobcode

  Fred @.id putn
  Sue @.id putn
  Sue .initials puts


---------
Functions
---------

+-------+-------+----------------------------------------------------------+
| Name  | Stack | Used for                                                 |
+=======+=======+==========================================================+
| {     | -n    | Start a new structure                                    |
+-------+-------+----------------------------------------------------------+
| field | nn"-n | Add a field to a structure. Takes the number of cells to |
|       |       | allocate and parses for a name                           |
+-------+-------+----------------------------------------------------------+
| fields| n"-n  | Add multiple fields to a structure. Takes the number of  |
|       |       | fields and parses for names and optionally sizes.        |
+-------+-------+----------------------------------------------------------+
| }     | n"-   | End a structure. Parses for the structure name           |
+-------+-------+----------------------------------------------------------+
| size  | a-n   | Return the size of a structure. The address must be the  |
|       |       | address of the function created by **}**, not the actual |
|       |       | allocated structure.                                     |
+-------+-------+----------------------------------------------------------+
| clone | a-a   | Given a structure address, return a new anonyous instance|
+-------+-------+----------------------------------------------------------+

