#! /bin/sh

for f in $1/*.rst
do
  rst2pdf --stylesheets doc/default.css $f
done
