=====
math'
=====
This library provides support for additional mathmatic operations not provided
by the core Retro language.


---------
Functions
---------
+------------+-------+-----------------------------------------------------+
| Function   | Stack | Description                                         |
+============+=======+=====================================================+
| squareRoot | x-n   | Find an (approximate) square root for a given value |
+------------+-------+-----------------------------------------------------+
| gcd        | xy-n  | Find the greatest common denominator for two values |
+------------+-------+-----------------------------------------------------+
| lcm        | xy-n  | Find the least common multiplier for two values     |
+------------+-------+-----------------------------------------------------+
| divisor?   | ab-f  | Tests to see if b is a divisor of a. Returns a flag.|
+------------+-------+-----------------------------------------------------+
| ``*/``     | abc-d | Multiply a by b, then divide bye c to get d         |
+------------+-------+-----------------------------------------------------+
| even?      | n-f   | Returns a flag indicating whether or not a number is|
|            |       | true                                                |
+------------+-------+-----------------------------------------------------+
| odd?       | n-f   | Returns a flag indicating whether or not a number is|
|            |       | false                                               |
+------------+-------+-----------------------------------------------------+
| pow        | bp-n  |  Raise (b) to power (p)                             |
+------------+-------+-----------------------------------------------------+
| abs        |  n-n  |  Absoulte value of number (n)                       |
+------------+-------+-----------------------------------------------------+
| min        | ab-c  |  Minimum of (a) or (b)                              |
+------------+-------+-----------------------------------------------------+
| max        | ab-c  |  Maximum of (a) or (b)                              |
+------------+-------+-----------------------------------------------------+
| random     |   -x  |  Return a random number                             |
+------------+-------+-----------------------------------------------------+


