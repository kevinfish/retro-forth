RX = ..

default: clean
	@echo Linking Retro from $(RX)
	@ln -s $(RX)/retro .
	@ln -s $(RX)/retroImage .
	@ln -s $(RX)/library .

clean:
	@echo Cleaning up old files...
	@rm -f retro retroImage library *~
