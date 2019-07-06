_: broadcast receive clean

broadcast: broadcast.c
	cl.exe /Zi /EHsc /I. /I.. broadcast.c

receive: receive.c
	cl.exe /Zi /EHsc /I. /I.. receive.c

clean:
	del *.ilk *.obj *.pdb

distclean: clean
	del *.exe
