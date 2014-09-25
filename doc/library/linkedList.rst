===========
linkedList'
===========

--------
Overview
--------
While linked lists are a simple data structure, they are very useful.
This vocabulary provides a foundation for creating and using them in
a predicatable, consistent way.

The **linkedList'** vocabulary provides support for single linked
lists.


Back Linked Lists
=================
In this form a list consists of a series of nodes that each point to
the prior node. The list pointer variable is updated with each **add**
or **remove** to point to the newest node in the chain.


--------
Examples
--------

Creating A List
===============
::

  ^linkedList'new: L


Adding A Value To A List
========================
::

  100 L ^linkList'add


Removing A Value From A List
============================
::

  L ^linkList'remove


Accessing A Specific Node
=========================
::

  10 L ^linkList'nth


---------
Functions
---------

+--------------+-------+-----------------------------------------------+
| Name         | Stack | Usage                                         |
+==============+=======+===============================================+
| node         | "-    | Structure for list nodes                      |
+--------------+-------+-----------------------------------------------+
| .prior       | a-a   | Access prior node field                       |
+--------------+-------+-----------------------------------------------+
| .value       | a-a   | Access value field                            |
+--------------+-------+-----------------------------------------------+
| new:         | "-    | Create a list. This gives a variable pointing |
|              |       | to a **node** structure                       |
+--------------+-------+-----------------------------------------------+
| add          | nL-   | Add a value to a list. Allocates a new node   |
+--------------+-------+-----------------------------------------------+
| remove       | L-n   | Remove a node from a list and return its value|
+--------------+-------+-----------------------------------------------+
| addNode      | NL-   | Add a node to a list                          |
+--------------+-------+-----------------------------------------------+
| removeNode   | L-N   | Remove a node from a list. Returns the node   |
+--------------+-------+-----------------------------------------------+
| nth          | nL-N  | Return the *nth* node from the list. Indexing |
|              |       | starts at *1*.                                |
+--------------+-------+-----------------------------------------------+

