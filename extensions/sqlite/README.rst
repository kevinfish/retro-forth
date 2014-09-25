sqlite bindings for Retro
=========================
This tool and vocabulary provide a bridge between an SQLite3 database and Retro.


Server
------
The server is written in Python.

* Load SQLite3 database *retro.db*
* Wait for a file named *query* to be created
  * When detected, load the query from a second file named *sql*
  * Erases the *query* file
* Perform the query on the sqlite database
* Write the results to a file named *results*
  * First line is an integer specifying the number of rows
  * Each result, one row per line.
  * Columns are tab separated
* Repeat until a *quit* file is created

Start the server by doing:

::

  python sqlite.py


Client
------
The client library is written in Retro. It provides a number of functions
under a **sql'** vocabulary.

+--------------+---------+-------------------------------------------------+
| Function     | Stack   | Description                                     |
+==============+=========+=================================================+
| submit       | [  -  ] | Create the *query* file                         |
+--------------+---------+-------------------------------------------------+
| submitQuery  | [ $-  ] | Store an SQL query and submit                   |
+--------------+---------+-------------------------------------------------+
| loadResults  | [  -n ] | Load the results of the query, return number of |
|              |         | results                                         |
+--------------+---------+-------------------------------------------------+
| getRow       | [  -$ ] | Read a row from the results                     |
+--------------+---------+-------------------------------------------------+
| finished     | [  -  ] | Close the results                               |
+--------------+---------+-------------------------------------------------+
| itemize      | [ $-n ] | Split a row into columns                        |
+--------------+---------+-------------------------------------------------+
| :column      | [ n-$ ] | Access a column from an itemized row            |
+--------------+---------+-------------------------------------------------+


Loading
+++++++

Option 1
~~~~~~~~
Put the *sql.rx* in the directory with your image, and do:

::

  include sql.rx

Option 2
~~~~~~~~
Put the *sql.rx* in your *library* directory, and do:

::

  needs sql'


Example
+++++++

::

  with sql'

  "select * from prices where item like 'SIEQ%'" submitQuery
  loadResults
    [ cr getRow itemize [ 1- :column "%s|" puts ] iter ] times
  finished

