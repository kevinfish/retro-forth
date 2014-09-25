# makefile for building retro image and some vm implementations

CFLAGS = -Wall

all: clean retro

retro:
	$(CC) $(CFLAGS) vm/complete/retro.c -o retro

python:
	cp vm/complete/retro.py retro
	chmod +x retro

ruby:
	cp vm/complete/retro.rb retro
	chmod +x retro

sbcl:
	( cd vm/complete/lisp && sbcl --no-sysinit --no-userinit --noprint --load sbcl.lisp )

.PHONY: image

image: retro
	cd image && cat meta.rx kernel.rx >../core.rx
	./retro --shrink --image retroImage --with core.rx
	rm core.rx

smallimage: retro
	cd image && cat meta.rx kernel.rx | grep -v \"\ \:doc >../core.rx
	cp retroImage smallRetroImage
	./retro --shrink --image smallRetroImage --with core.rx
	rm core.rx


helpful: image
	./retro --image retroImage --with image/helpful.rx

docstrings:
	cp retroImage helpImage
	./retro --image helpImage --with tools/exportDocstrings.rx
	./retro --image helpImage >doc/Function_List.md
	rm helpImage

jsimage:
	./retro --with vm/web/html5/dumpImage.rx
	cp retroImage.js vm/web/android-phonegap/assets/www
	mv retroImage.js vm/web/html5

images:
	$(CC) $(CFLAGS) tools/convert.c -o convert
	./convert
	rm -f convert

clean:
	rm -f retro
	rm -f retroImage16 retroImage64
	rm -f retroImage16BE retroImageBE retroImage64BE
	rm -f *~

cleaner:
	rm -f `find . | grep \~`

webdocs:
	sh tools/exportHTML.sh doc
	mv doc/*.html www/docs
	sh tools/exportHTML.sh doc/library
	mv doc/library/*.html www/docs/library

libdocs:
	sh tools/extractDocs.sh library
	mv library/*.rst doc/library
	cat doc/library/*.rst >doc/Libraries.rst

