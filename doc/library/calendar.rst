=========
calendar'
=========


--------
Overview
--------
This vocabulary provides support for doing calculations involving time
and dates.


---------
Functions
---------
+-----------------+-------+--------------------------------------+
| Function        | Stack | Used For                             |
+=================+=======+======================================+
| MINUTES/WEEK    | -n    | Number of minutes in a week          |
+-----------------+-------+--------------------------------------+
| MINUTES/DAY     | -n    | Number of minutes in a day           |
+-----------------+-------+--------------------------------------+
| MINUTES/HOUR    | -n    | Number of minutes in an hour         |
+-----------------+-------+--------------------------------------+
| HOURS/DAY       | -n    | Number of hours in a day             |
+-----------------+-------+--------------------------------------+
| DAYS/WEEK       | -n    | Number of days in a week             |
+-----------------+-------+--------------------------------------+
| DAYS/YEAR       | -n    | Number of days in a typical year     |
+-----------------+-------+--------------------------------------+
| WEEKS/YEAR      | -n    | Number of weeks in a year            |
+-----------------+-------+--------------------------------------+
| SECONDS/MINUTE  | -n    | Number of seconds in a minute        |
+-----------------+-------+--------------------------------------+
| SECONDS/HOUR    | -n    | Number of seconds in an hour         |
+-----------------+-------+--------------------------------------+
| SECONDS/DAY     | -n    | Number of seconds in a day           |
+-----------------+-------+--------------------------------------+
| dayToName       | n-$   | Convert the number of a day to a     |
|                 |       | three letter abbreviation            |
+-----------------+-------+--------------------------------------+
| daysPerMonth    | m-n   | Return the number of days in a given |
|                 |       | month                                |
+-----------------+-------+--------------------------------------+
| secondsPerMonth | m-n   | Return the number of seconds in a    |
|                 |       | given month                          |
+-----------------+-------+--------------------------------------+
| isLeapYear?     | y-f   | Return true (-1) if the year is a    |
|                 |       | leap year or false (0) otherwise     |
+-----------------+-------+--------------------------------------+
| secondsPerYear  | y-n   | Return the number of seconds in a    |
|                 |       | given year                           |
+-----------------+-------+--------------------------------------+
| toJulian        | dmy-j | Convert a date into a Julian day     |
+-----------------+-------+--------------------------------------+
| toDayOfWeek     | dmy-n | Convert a date to the numeric day of |
|                 |       | the week                             |
+-----------------+-------+--------------------------------------+


