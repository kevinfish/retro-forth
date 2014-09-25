==========
assertion'
==========


--------
Overview
--------
This vocabulary provides support for testing code in a clean,
predicatable manner.

Assertion predicates first check the stack for underflow; if
the stack is deep enough, their embedded predicates are applied;
if not, the assertion fails.

The result of each assertion - including the underflow check -
is ANDed with the assertionFlag which can then be tested after
the containing thread has finished executing; this is handled by
the .assertion word class.

For custom behaviour, revector preCond and/or postCond; by
default the pre-condition is an effective nop while the post-
condition simply prints 'Success' or 'Failure'.

Given that each assertion predicate mutates assertionFlag, use
of the word class .assertion is encouraged; this resets the
assertionFlag before execution and push its final value to the
stack before calling postCond when the thread exits.

NOTE: For simplicity of implementation, failure within a word of
class .assertion will not result in immediate termination;
instead, the false value of assertionFlag is left to propagate.


--------
Examples
--------
::

  ( Our assertions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ )

  ( n.b, underflow is dealt with sanely )
  : a1 ( xyz- ) 30 assert= 20 assert= 10 assert= ; assertion
  : a2 (  xy- ) assertTrue assertFalse ; assertion
  : a3 (   x- ) 5 assert< ; assertion

  ( Simple demo ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ )
  : go
    "\n----------" puts
    10 20 30 a1
    30 20 10 a1
    "\n----------" puts
    10 a1
    "\n----------" puts
    a1
    "\n----------" puts
    -1 0 a2
    0 -1 a2
    -1 -1 a2
    -1 a2
    "\n----------" puts
    3 a3
    4 a3
    5 a3
    a3
    "\n----------" puts
  ;

  clear go

  ( Adjusting pre- and post- conditions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ )
  : myPreCond .s ;
  : myPostCond [ "Fatal error: assertion failure." puts bye ] ifFalse ;
  &myPostCond is postCond
  &myPreCond is preCond

  ( 'go' will now exit on first failure )
  go


---------
Functions
---------

+---------------+-------+-----------------------------------------------------+
| Name          | Stack | Usage                                               |
+===============+=======+=====================================================+
| assertionFlag | -a    | Variable. This holds a true (-1) or false (0)       |
|               |       | value indicating whether the current set of         |
|               |       | assertions have passed or failed. The **.assertion**|
|               |       | class will set this to true automatically.          |
+---------------+-------+-----------------------------------------------------+
| assert        | f-    | Updates the **assertionFlag** using bitwise AND     |
+---------------+-------+-----------------------------------------------------+
| available     | n-f   | Checks to see if there are at least *n* items on the|
|               |       | stack. Returns true if there are, or false otherwise|
+---------------+-------+-----------------------------------------------------+
| assert=       | nn-   | Check to see if two values are equal                |
+---------------+-------+-----------------------------------------------------+
| assertFalse   | f-    | Check to see if flag is false (0)                   |
+---------------+-------+-----------------------------------------------------+
| assertTrue    | f-    | Check to see if flag is true (non-zero)             |
+---------------+-------+-----------------------------------------------------+
| assert>       | nn-   | Check to see if n1 is greather than n2              |
+---------------+-------+-----------------------------------------------------+
| assert>=      | nn-   | Check to see if n1 is greater than or equal to n2   |
+---------------+-------+-----------------------------------------------------+
| assert<       | nn-   | Check to see if n1 is less than n2                  |
+---------------+-------+-----------------------------------------------------+
| assert<=      | nn-   | Check to see if n1 is less than or equal to n2      |
+---------------+-------+-----------------------------------------------------+
| putAssertion  | f-    | Display 'Success' or 'Failure' based on flag        |
+---------------+-------+-----------------------------------------------------+
| preCond       | ``-`` | Hook, does nothing bu default                       |
+---------------+-------+-----------------------------------------------------+
| postCond      | f-    | Hook, displays 'Success' or 'Failure' by default    |
+---------------+-------+-----------------------------------------------------+
| .assertion    | a-    | Class handler for assertions                        |
+---------------+-------+-----------------------------------------------------+
| assertion     | ``-`` | Change the class of a function to **.asssertion**   |
+---------------+-------+-----------------------------------------------------+

