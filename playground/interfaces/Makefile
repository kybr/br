_: list clean

list: list.c
	cl.exe /Zi /EHsc /I. /I.. list.c

clean:
	del *.ilk *.obj *.pdb

distclean: clean
	del *.exe
