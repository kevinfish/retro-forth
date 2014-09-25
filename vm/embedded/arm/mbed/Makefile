default: clean
	ln -s ../../../../retro .
	ln -s ../../../../library .
	cp ../../../../retroImage .
	./retro --with image.rx --shrink
	$(CC) ../../../../tools/convert.c -o convert
	./convert
	rm -f convert retroImage*BE retroImage64 retroImage
	mv retroImage16 retroImg

clean:
	rm -f retro retroImage library
