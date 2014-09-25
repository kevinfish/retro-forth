#!/bin/sh
#
# creates a runnable jar "retro.jar", run like this:
#
# java -jar retro.jar
#
# and currently it will run all tests successfully (run from
# top-level project directory)
#
mkdir -p javabuild
cd java
find . -name "*.java" > javasrc.txt
javac -d ../javabuild @javasrc.txt
rm javasrc.txt
cd ..
echo "Manifest-Version: 1.0" > manifest.txt
echo "Created-By: 1.6.0 (Sun Microsystems Inc.)" >> manifest.txt
echo "Main-Class: com.xoba.ngaro.NGaroDriver" >> manifest.txt
cd javabuild
jar cfm ../retro.jar ../manifest.txt .
cd ..
rm manifest.txt
rm -rf javabuild
