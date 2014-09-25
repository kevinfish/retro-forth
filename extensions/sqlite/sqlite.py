# rxSQL ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Copyright (c) 2010 - 2011, Charles Childers
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Filesystem based interface to sqlite3
#
# Usage
#  - Write SQL query to file named 'sql'
#  - Create a file named 'query'
#  - Read results from file named 'results'
#     - First line is integer specifying number of rows
#     - Rest is db results, in tab separated value format
#  - When finished, create a file named 'quit'
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

import os, sys, math, time, struct, sqlite3
a = True
db = sqlite3.connect('retro.db')
c  = db.cursor()

while a == True:
  if os.path.exists("query") and os.path.exists("sql"):
    os.remove("query")
    if os.path.exists("results"):
      os.remove("results")
    f = open("sql", "r")
    query = f.read()
    f.close()
    f = open("results0", "w")
    c.execute("select count(*) as count from (" + query + ")")
    for row in c:
      f.write(str(row[0]) + "\n")
    c.execute(query)
    for row in c:
      for item in row:
        f.write(str(item) + "\t")
      f.write("\n")
    f.close()
    db.commit()
    if os.path.exists("sql"):
      os.remove("sql")

  if os.path.exists("results0"):
    os.rename("results0", "results")

  if os.path.exists("quit"):
    os.remove("quit")
    a = False
