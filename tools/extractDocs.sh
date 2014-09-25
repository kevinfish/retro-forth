#! /bin/sh

for f in $1/*.rx
do
  echo getDocsFrom $f | ./retro --with tools/extractDocs.rx
done

perl -e'unlink grep !-s, @ARGV' $1/*.rst
