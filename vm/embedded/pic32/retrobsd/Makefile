default: clean
	ln -s ../../../../retro .
	ln -s ../../../../library .
	cp ../../../../retroImage .
	./retro --with image.rx --shrink
	mv retroImage retro-src/retroImage

clean:
	rm -f retro library
	rm -f retro-src/retroImage
