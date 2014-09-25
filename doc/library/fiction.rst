========
fiction'
========

--------
Overview
--------

I have always enjoyed playing interactive fiction games. This library provides a
framework for building simple games of this type. It's been used for an implemention
of *Cloak of Darkness*, as well as various small puzzle games for my children.


---------
Functions
---------

+-------------------+-----------+------------------------------------------------+
| Name              | Stack     | Usage                                          |
+===================+===========+================================================+
| ROOM              | -n        | Item type for rooms                            |
+-------------------+-----------+------------------------------------------------+
| ITEM              | -n        | Item type for items                            |
+-------------------+-----------+------------------------------------------------+
| .description      | a-a       | Object field. Describe the item                |
+-------------------+-----------+------------------------------------------------+
| .shortDescription | a-a       | Object field. Short name for object            |
+-------------------+-----------+------------------------------------------------+
| .type             | a-a       | Object field. Type. ROOM or ITEM               |
+-------------------+-----------+------------------------------------------------+
| .onLook           | a-a       | Object field. Custom handler for "look"        |
+-------------------+-----------+------------------------------------------------+
| .onEntry          | a-a       | Object field. Custom handler for entry event   |
+-------------------+-----------+------------------------------------------------+
| .location         | a-a       | Object field. Where is this object?            |
+-------------------+-----------+------------------------------------------------+
| .visited          | a-a       | Object field. Have we been here before?        |
+-------------------+-----------+------------------------------------------------+
| .toNorth          | a-a       | Object field. Pointer to location to the north |
+-------------------+-----------+------------------------------------------------+
| .toSouth          | a-a       | Object field. Pointer to location to the south |
+-------------------+-----------+------------------------------------------------+
| .toEast           | a-a       | Object field. Pointer to location to the east  |
+-------------------+-----------+------------------------------------------------+
| .toWest           | a-a       | Object field. Pointer to location to the west  |
+-------------------+-----------+------------------------------------------------+
| .onNorth          | a-a       | Object field. Custom handler for "north"       |
+-------------------+-----------+------------------------------------------------+
| .onSouth          | a-a       | Object field. Custom handler for "south"       |
+-------------------+-----------+------------------------------------------------+
| .onEast           | a-a       | Object field. Custom handler for "east"        |
+-------------------+-----------+------------------------------------------------+
| .onWest           | a-a       | Object field. Custom handler for "west"        |
+-------------------+-----------+------------------------------------------------+
| .postDescription  | a-a       | Object field. Custom handler run after showing |
|                   |           |               a description                    |
+-------------------+-----------+------------------------------------------------+
| .preDescription   | a-a       | Object field. Custom handler run before showing|
|                   |           |               a desription                     |
+-------------------+-----------+------------------------------------------------+
| .onRead           | a-a       | Object field. custom handler for "read"        |
+-------------------+-----------+------------------------------------------------+
| describes         | $"-       | Add a description to an object                 |
+-------------------+-----------+------------------------------------------------+
| nameOf            | $"-       | Specify a short name for an object             |
+-------------------+-----------+------------------------------------------------+
| room              | "-        | Create a new ROOM object                       |
+-------------------+-----------+------------------------------------------------+
| invertDirection   | a$-a      |                                                |
+-------------------+-----------+------------------------------------------------+
| is                | a""""-    |                                                |
+-------------------+-----------+------------------------------------------------+
| items             | -L        | Linked list of all known items                 |
+-------------------+-----------+------------------------------------------------+
| item              | "-        | Create a new ITEM object                       |
+-------------------+-----------+------------------------------------------------+
| contains          | a"-       | Mark that a room contains an item              |
+-------------------+-----------+------------------------------------------------+
| has?              | a-af      | Does the player have an item?                  |
+-------------------+-----------+------------------------------------------------+
| displayItem       | a-        | Display the name of an item                    |
+-------------------+-----------+------------------------------------------------+
| inventory         | ``-``     | Display all items the player has               |
+-------------------+-----------+------------------------------------------------+


