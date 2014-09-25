# this file uses ngarotest.py to test various ngaro interpreters
PYTHON = python
NGTEST = $(PYTHON) ./ngarotest.py
VMPATH = ../../vm
RX.PY  = $(VMPATH)/complete/retro.py 
RX.C   = ../../retro

test.all: test.py test.c test.pas

test.py:
	chmod u+x $(RX.PY)
	$(NGTEST) -n $(RX.PY)

test.c:
	$(NGTEST) -n $(RX.C)

clean:
	rm -f *.img


# for the moment, my pascal version is in another repository
# ( https://github.com/sabren/b4/ )
B4PATH = ~/b
test.pas:
#	fpc -gl -B $(B4PATH)/retro.pas
	$(NGTEST) -n $(B4PATH)/retro
