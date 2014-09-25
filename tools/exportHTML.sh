#! /bin/sh

for f in $1/*.rst
do
  rst2html.py --stylesheet /documentation.css --link-stylesheet $f >${f%.*}.html
done
